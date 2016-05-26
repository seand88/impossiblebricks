#include "GameScene.h"
#include "B2DebugDrawLayer.h"
#include "LevelManager.h"
#include "LevelSelectScene.h"
#include "Brick.h"
#include "HelloWorldScene.h"
#include "LevelFailMenu.h"
#include "LevelWinMenu.h"
#include "SaveManager.h"
#include "TutorialScreen.h"
#include "LevelPauseMenu.h"

USING_NS_CC;

#define SCALE_RATIO 32.0
#define NUM_PREDICTION_POINTS 15

GameScene* GameScene::create(int level)
{ 
	GameScene *pRet = new GameScene(); 
	pRet->m_level = level;

	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease();
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

Scene* GameScene::createScene(int level)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create(level);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	levelComplete = false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundGraphic = Sprite::create("vertvel_gameBorder.png");
	backgroundGraphic->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(backgroundGraphic, 0);

	dropZone = Sprite::create("vertvel_dropZone.png");
	dropZone->setPosition(visibleSize.width/2, visibleSize.height - 100);
	this->addChild(dropZone, 0);

    //SET MOUSE LISTENER
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //END MOUSE LISTENER
	
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);  
	world->SetContactListener(this);

    //CREATE A BALL
    dragOffsetStartX = 0;    
    dragOffsetEndX = 0;    
    dragOffsetStartY = 0;    
    dragOffsetEndY = 0;    
    existBall= false;    
    ballX = 320;
    ballY = 900;  
    powerMultiplier = 3.5;  
	
	ball = new GameObject(GameObject::ObjectType::BALL, "green_ball.png");
	ball->m_sprite->setPosition(Point(ballX,ballY));
	this->addChild(ball->m_sprite);    
	ball->m_sprite->setVisible(false);

    addWall(visibleSize.width ,10,(visibleSize.width / 2) ,visibleSize.height); //CEIL
    addWall(10 ,visibleSize.height ,0,(visibleSize.height / 2) ); //LEFT
    addWall(10 ,visibleSize.height ,visibleSize.width,(visibleSize.height / 2) ); //RIGHT

	LevelManager::getInstance().loadLevel(m_level);

	//create the bricks
	std::vector<LevelManager::BrickData> brickList = LevelManager::getInstance().m_bricks;
	for(LevelManager::BrickData brickData : brickList)
	{
		addBrick(brickData.x, brickData.y, brickData.type);
	}

	//TODO: Finish Logic for the Warp
	//addWarp(300,300);

	//init max balls and balls remaining
	m_maxBalls = LevelManager::getInstance().m_maxBalls;
	m_ballsRemaining = m_maxBalls;

	log("[GameScene] Max Balls %d", m_maxBalls);

	//create the UI
	createUI();

    scheduleUpdate();

	ballReady = true;
	isTouching = false;
	m_hits = 0;

	//addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 9999);

	//add dots
	for (int i = 1 ; i <= NUM_PREDICTION_POINTS; i++){
		points[i] =CCSprite::create("vertvel_ballTrack.png");
		points[i]->setVisible(false);
        this->addChild(points[i]); 
    }

	//add the pause button
	using namespace cocos2d::ui;
	auto exitBtn = Button::create();
    exitBtn->setTouchEnabled(true);
    exitBtn->loadTextures("vv_pause.png", "vv_pause.png", "");
    exitBtn->setPosition(Point(50,75));
    exitBtn->addTouchEventListener(this, toucheventselector(GameScene::exit));
	exitBtn->setTag(0);
	this->addChild(exitBtn);

	//add the tutorial screen
	auto tutorialScreen = TutorialScreen::create(m_level);
	addChild(tutorialScreen);

    return true;
}

void GameScene::moveBall(float x, float y)
{
	if (ball && ball->m_physicsBody)
	{
		ball->m_physicsBody->SetTransform(b2Vec2(x/SCALE_RATIO,y/SCALE_RATIO), ball->m_physicsBody->GetAngle());
	}
}

void GameScene::exit(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				addChild(LevelPauseMenu::create());
                break;
                
            default:
                break;
        }
}

//Simulate Physics
void GameScene::update(float dt){
	if (levelComplete)
			return;

   int positionIterations = 10;  
   int velocityIterations = 10;
   
   deltaTime = dt;
   world->Step(dt, velocityIterations, positionIterations);  
  
   //first remove any duplicates...
   std::sort( bodiesToRemove.begin(), bodiesToRemove.end() );
   bodiesToRemove.erase( std::unique( bodiesToRemove.begin(), bodiesToRemove.end() ), bodiesToRemove.end() );
   //now destroy them
   for(b2Body* bodyToRemove : bodiesToRemove)
   {
	   if (bodyToRemove->GetUserData()) {
		   GameObject *gameobject = static_cast<GameObject*>(bodyToRemove->GetUserData());
		   if (gameobject->m_objectType == GameObject::ObjectType::BRICK)
		   {
			   if (m_hits > 1)
			   {
				   showFadingHitLabel(gameobject->m_sprite->getPosition().x, gameobject->m_sprite->getPosition().y);
			   }

			   //add to score if brick was hit
			   int scoreToAdd = 100 + (m_hits * (100 * 1.5f));
			   m_score += scoreToAdd;
			   m_hits++; //add one to hits
		   }

		   world->DestroyBody(bodyToRemove);
	   }
   }
   //clear bodies to remove vector
   bodiesToRemove.clear();

   for (Vec2 position : m_explosions)
   {
	   this->createBrickExplosion(position);
   }
   //clear the explosions
   m_explosions.clear();

   //if we hit a warp, warp to a position and then clear the vector
   for (b2Body* warp : m_warpsHit)
   {
	   moveBall(300,700); //just test numbers for now
	   break;
   }
   m_warpsHit.clear();

   
   bool foundBrick = false;
   for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
   {
	   if (body->GetUserData()) 
	   {  
		  GameObject *gameobject = (GameObject *) body->GetUserData();  
		  if (gameobject->m_hasSprite == true)
		  {
			  gameobject->update();
		  }

		  if(gameobject->m_objectType == GameObject::ObjectType::BRICK)
		  {
			Brick* brick = static_cast<Brick*>(gameobject);
			if(brick->m_brickType != Brick::BrickType::SOLID)//ignore solid bricks as they dont end the level
				foundBrick = true;
		  }
		  else if (gameobject->m_objectType == GameObject::ObjectType::BALL)
		  {
			  //if balls go off screen, delete them unless its the main game ball we re-use
			  if (gameobject->m_sprite->getPosition().y < 0 && ball != gameobject) 
			  {
				  m_exploBalls.pop_back();
				  bodiesToRemove.push_back(body);//schedule for deletion
				  gameobject->m_sprite->setVisible(false);
				  gameobject->m_sprite = nullptr;
				  gameobject->m_physicsBody = nullptr; 
			  }
		  }
	   }  
   }

   world->ClearForces();
   world->DrawDebugData();     

   if (ballReady == false){
	 if (ballTrail && ball->m_sprite) {
			ballTrail->setPosition(ball->m_sprite->getPosition());
	   }
   }
   
   //clean up the ball trail
   if (ballReady == true)
   {
	   if (ballTrail) {
			removeChild(ballTrail);
	   }
   }

   if(foundBrick == false) //no bricks left, level is complete!
   {
	    //check if the ball is ready!
		if (ballReady == false && ball && ball->m_sprite->getPosition().y < 0 && m_exploBalls.size() <= 0)
		{
			levelComplete = true;
			//won the level
			showLevelComplete(true);
			return;
		}
   }
   else if (m_ballsRemaining <= 0)
   {
		if (ballReady == false && ball && ball->m_sprite->getPosition().y < 0 && m_exploBalls.size() <= 0)
		{
			levelComplete = true;
			//lost the level!
			showLevelComplete(false);
			return;
		}
   }

   //set linear dampening on the ball
   if(ballReady == false && ball && ball->m_hasPhysics)
   {
	   ball->m_physicsBody->SetLinearDamping(0.1f);
   }

   updateUI();
}  

void GameScene::showFadingHitLabel(float x, float y)
{
	//log("LABEL X: %f, Y: %f", x, y);

	TTFConfig label_config_names;
	label_config_names.fontFilePath = "absender1.ttf";
	label_config_names.fontSize = 28;

	auto scoreNameLbl = Label::createWithTTF(label_config_names, "X" + std::to_string(m_hits));
	scoreNameLbl->setPositionX(x);
	scoreNameLbl->setPositionY(y);
	scoreNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(scoreNameLbl);

	//fade the label
	FadeOut *fade = FadeOut::create(1.0f);
	scoreNameLbl->runAction(fade);
}

void GameScene::showLevelComplete(bool won)
{
	//remove the trail
	if (ballTrail) {
		ballTrail->stopSystem();
	}

	if (won)
	{
		//we won, show win screen
		addChild(LevelWinMenu::create(m_ballsRemaining, m_score));
	}
	else
	{
		//we lost show lose screen!
		addChild(LevelFailMenu::create());
	}

}

void GameScene::addBrick(float x, float y, int type)
{
	auto brick = new Brick(type);
	brick->m_sprite->setPosition(Point(x, y));
	brick->m_sprite->setTag(type);
	brick->createPhysicsBody(world, x, y);
	this->addChild(brick->m_sprite, 0);
}

void GameScene::addWarp(float x, float y)
{
	auto warp = new Warp();
	warp->m_sprite->setPosition(Point(x, y));
	warp->m_sprite->setTag(0);
	warp->createPhysicsBody(world, x, y);
	this->addChild(warp->m_sprite);
}

void GameScene::addWall(float w,float h,float px,float py) {

	GameObject* wallObj = new GameObject(GameObject::ObjectType::WALL);

    b2PolygonShape floorShape;

    floorShape.SetAsBox(w/ SCALE_RATIO,h/ SCALE_RATIO);
    b2FixtureDef floorFixture;
    floorFixture.density=1;
    floorFixture.friction=0.3;
    floorFixture.restitution=1;
    floorFixture.shape=&floorShape;
	floorFixture.filter.categoryBits = GameObject::CollisionCategory::WALL1;
    b2BodyDef floorBodyDef;

    floorBodyDef.position.Set(px/ SCALE_RATIO,py/ SCALE_RATIO);
    b2Body *floorBody = world->CreateBody(&floorBodyDef);
	floorBody->SetUserData(wallObj);
    floorBody->CreateFixture(&floorFixture);

	wallObj->m_physicsBody = floorBody;
	wallObj->m_hasPhysics = true;
}

void GameScene::defineBall(){
    m_ballShape.m_radius = 10 / SCALE_RATIO;
    b2FixtureDef ballFixture;
    ballFixture.density=4;
    ballFixture.friction=2;
    ballFixture.restitution=0.6;
    ballFixture.shape=&m_ballShape;
	ballFixture.filter.categoryBits = GameObject::CollisionCategory::BALL1;
	ballFixture.filter.maskBits = GameObject::CollisionCategory::BRICK1 | GameObject::CollisionCategory::WALL1 | GameObject::CollisionCategory::WARP1;

    m_ballBodyDef.type= b2_dynamicBody;
    m_ballBodyDef.position.Set(ball->m_sprite->getPosition().x/SCALE_RATIO,ball->m_sprite->getPosition().y/SCALE_RATIO);

    auto ballBody = world->CreateBody(&m_ballBodyDef);
	ballBody->SetUserData(ball);
    ballBody->CreateFixture(&ballFixture);
    ballBody->SetGravityScale(3);

	ball->m_physicsBody = ballBody;
	ball->m_hasPhysics = true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	Point touchLocation = touch->getLocation();
	//find out if we are touching the dropzone or not....
	if(!dropZone->boundingBox().containsPoint(touchLocation))
	{
		return false; //only let touches happen in the drop zone!
	}

	if (m_ballsRemaining <= 0 )
		return false;

	if (isTouching)
		return false;

	isTouching = true;

   //check if the ball is ready!
   if (ballReady == false && ball && ball->m_sprite->getPosition().y < -200 && m_exploBalls.size() <= 0){
		ballReady = true; //can shoot next ball!
   }

	if(ballReady == false)
			return true;

    dragOffsetStartX = touchLocation.x;
    dragOffsetStartY = touchLocation.y;

    ballX = touchLocation.x;
    ballY = touchLocation.y;

    if (existBall)
	{
		ball->m_sprite->setVisible(false);
		world->DestroyBody(ball->m_physicsBody);
		ball->m_hasPhysics = false;
		existBall = false;
    }

	ball->m_sprite->setVisible(true);
    ball->m_sprite->setPosition(Vec2(ballX ,ballY));
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	if(ballReady == false)
			return;

    Point touchLocation = touch->getLocation();

    dragOffsetEndX = touchLocation.x;
    dragOffsetEndY = touchLocation.y;

    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

	GameScene::simulateTrajectory(b2Vec2((dragDistanceX * powerMultiplier)/SCALE_RATIO,(dragDistanceY * powerMultiplier)/SCALE_RATIO));
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	isTouching = false;

	if(ballReady == false)
		return;

	ballReady = false; //cant shoot another ball until its ready
    existBall = true;

    GameScene::defineBall();

	Point touchLocation = touch->getLocation();

    dragOffsetEndX = touchLocation.x;
    dragOffsetEndY = touchLocation.y;

    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

	ball->m_physicsBody->SetLinearVelocity(b2Vec2((dragDistanceX * powerMultiplier)/SCALE_RATIO,(dragDistanceY * powerMultiplier)/SCALE_RATIO));

	//attach particle to the ball!
	ballTrail = CCParticleSystemQuad::create("particles/particle_ball.plist");
	ballTrail->setDuration(-1);
	ballTrail->setPosition(ball->m_sprite->getPosition());
	this->addChild(ballTrail);

	//remove points
	for (int i = 1 ; i <= NUM_PREDICTION_POINTS; i++){
		points[i]->setVisible(false); 
    }

	m_hits = 0; //reset hits
	m_ballsRemaining--;
} 

void GameScene::simulateTrajectory(b2Vec2 coord)
{
    //define ball physicis
    GameScene::defineBall();

    ball->m_physicsBody->SetLinearVelocity(b2Vec2(coord.x,coord.y));

    for (int i = 1; i <= NUM_PREDICTION_POINTS; i++)
	{
		world->Step(0.02f, 5 ,5 );
		points[i]->setPosition(Point(ball->m_physicsBody->GetPosition().x*SCALE_RATIO,ball->m_physicsBody->GetPosition().y*SCALE_RATIO));
		points[i]->setVisible(true);	
    }

	world->ClearForces();

    world->DestroyBody(ball->m_physicsBody);
	ball->m_hasPhysics = false;
}

void GameScene::BeginContact(b2Contact* contact)
{
  if (existBall && contact && contact->IsTouching())
  {
	  GameObject* objectA = static_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData());
	  GameObject* objectB = static_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData());

	  int typeA = objectA->m_objectType;
	  int typeB = objectB->m_objectType;

	  //ball hit a brick, destroy the brick!
	  if (typeA == GameObject::ObjectType::BALL && typeB == GameObject::ObjectType::BRICK )
	  {
		Brick* brickB = static_cast<Brick *>(objectB);
		brickB->hit();
		if (brickB->isDead())
		{
			if (brickB->m_brickType == Brick::BrickType::EXPLODING){
				m_explosions.push_back(objectB->m_sprite->getPosition());
			}

			this->bodiesToRemove.push_back(contact->GetFixtureB()->GetBody());
			objectB->m_sprite->setVisible(false);

			ParticleSystemQuad *explosion = ParticleSystemQuad::create("particles/particle_brickdestroy.plist");
			//explosion->setEmissionRate(800);
			explosion->setPosition(objectB->m_sprite->getPosition());
			this->addChild(explosion);
			explosion->setAutoRemoveOnFinish(true);
		}

	  }
	  else if (typeA == GameObject::ObjectType::BRICK && typeB == GameObject::ObjectType::BALL )
	  {
		Brick* brickA = static_cast<Brick *>(objectA);
		brickA->hit();
		if (brickA->isDead())
		{
			if (brickA->m_brickType == Brick::BrickType::EXPLODING){
				m_explosions.push_back(objectB->m_sprite->getPosition());
			}

			this->bodiesToRemove.push_back(contact->GetFixtureA()->GetBody());
			objectA->m_sprite->setVisible(false);

			ParticleSystemQuad *explosion = ParticleSystemQuad::create("particles/particle_brickdestroy.plist");
			//explosion->setEmissionRate(800);
			explosion->setPosition(objectA->m_sprite->getPosition());
			this->addChild(explosion);
			explosion->setAutoRemoveOnFinish(true);
		}

	  }
	  else if (typeA == GameObject::ObjectType::BALL && typeB == GameObject::ObjectType::WARP)
	  {
		 this->m_warpsHit.push_back(contact->GetFixtureB()->GetBody());
		 log("HIT THE WARP!");
	  }
	  else if (typeA == GameObject::ObjectType::WARP && typeB == GameObject::ObjectType::BALL)
	  {
		  this->m_warpsHit.push_back(contact->GetFixtureA()->GetBody());
		  log("HIT THE WARP!");
	  }
  }
}

void GameScene::EndContact(b2Contact* contact)
{
  // nothing to do here for the current implementation
	
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ::exit(0);
#endif
}

//sets up the UI
void GameScene::createUI()
{
	m_score = 0;

	//create names for the score and balls left
	TTFConfig label_config_names;
	label_config_names.fontFilePath = "rainfall.otf";
	label_config_names.fontSize = 28;

	auto scoreNameLbl = Label::createWithTTF(label_config_names, "score");
	scoreNameLbl->setPositionX(70);
	scoreNameLbl->setPositionY(25);
	scoreNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(scoreNameLbl);

	auto ballsNameLbl = Label::createWithTTF(label_config_names, "balls");
	ballsNameLbl->setPositionX(510);
	ballsNameLbl->setPositionY(25);
	ballsNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(ballsNameLbl);

	//actual balls left and score is in a different font
	TTFConfig label_config;
	label_config.fontFilePath = "draco.ttf";
	label_config.fontSize = 28;

	m_scoreLbl = Label::createWithTTF(label_config, "");
	m_scoreLbl->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	m_scoreLbl->setWidth(200);
	m_scoreLbl->setPositionX(scoreNameLbl->getPositionX() + 170);
	m_scoreLbl->setPositionY(scoreNameLbl->getPositionY() + 5);
	addChild(m_scoreLbl);	

	m_ballsLbl = Label::createWithTTF(label_config, "");
	m_ballsLbl->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	m_ballsLbl->setWidth(100);
	m_ballsLbl->setPositionX(ballsNameLbl->getPositionX() + 120);
	m_ballsLbl->setPositionY(ballsNameLbl->getPositionY() + 5);
	addChild(m_ballsLbl);

	updateUI();
}

void GameScene::updateUI()
{
	m_scoreLbl->setString(std::to_string(m_score));
	m_ballsLbl->setString(std::to_string(m_ballsRemaining));
}

void GameScene::createBrickExplosion(cocos2d::Vec2 position)
{
	for (int i = 0; i < 5; i++)
	{
		GameObject* exploBall = new GameObject(GameObject::ObjectType::BALL, "red_ball.png");
		exploBall->m_sprite->setPosition(position);
		
		b2CircleShape ballShape;
		b2BodyDef	  ballBodyDef; 
		b2FixtureDef  ballFixture;

		//set physics to the ball
		ballShape.m_radius = 10 / SCALE_RATIO;
		
		ballFixture.density=4;
		ballFixture.friction=2;
		ballFixture.restitution=0.6;
		ballFixture.shape=&ballShape;
		ballFixture.filter.categoryBits = GameObject::CollisionCategory::BALL1; // category
		ballFixture.filter.maskBits = GameObject::CollisionCategory::BRICK1 | GameObject::CollisionCategory::WALL1; //categories collides with

		ballBodyDef.type= b2_dynamicBody;
		ballBodyDef.position.Set(exploBall->m_sprite->getPosition().x/SCALE_RATIO,exploBall->m_sprite->getPosition().y/SCALE_RATIO);

		auto ballBody = world->CreateBody(&ballBodyDef);
		ballBody->SetUserData(exploBall);
		ballBody->CreateFixture(&ballFixture);
		ballBody->SetGravityScale(3);

		exploBall->m_physicsBody = ballBody;
		exploBall->m_hasPhysics = true;

		//add the sprite as a child
		this->addChild(exploBall->m_sprite);  

		//fire the ball
		int randX = rand() % 100 + 50;
		int randY = rand() % 100 + 50;
		int negative = rand() % 100 + 1;
		if (negative > 50){
			randX = -randX;
			randY = -randY;
		}

		exploBall->m_physicsBody->SetLinearVelocity(b2Vec2((randX * powerMultiplier)/SCALE_RATIO,(randY * powerMultiplier)/SCALE_RATIO));

		m_exploBalls.push_back(exploBall);
	}
}
