#include "LevelEditor.h"
#include "LevelManager.h"
#include "HelloWorldScene.h"

USING_NS_CC;

LevelEditor* LevelEditor::create(int level)
{ 
	LevelEditor *pRet = new LevelEditor(); 
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

Scene* LevelEditor::createScene(int level)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LevelEditor::create(level);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelEditor::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_lastBrick = nullptr;
	m_brickType = Brick::BrickType::NORMAL;
	m_oneStarScore = 0;
	m_twoStarScore = 0;
	m_threeStarScore = 0;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelEditor::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelEditor::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this); 

	auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LevelEditor::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	dropZone = Sprite::create("vertvel_dropZone.png");
	dropZone->setPosition(visibleSize.width/2, visibleSize.height - 100);
	this->addChild(dropZone, 0);
	
	using namespace cocos2d::ui;

	auto exitBtn = Button::create();
    exitBtn->setTouchEnabled(true);
    exitBtn->loadTextures("blue_button.png", "blue_button.png", "");
    exitBtn->setPosition(Point(100,15));
    exitBtn->addTouchEventListener(this, toucheventselector(LevelEditor::exitLevel));
	exitBtn->setTitleText("Exit");
	exitBtn->setTag(0);
	this->addChild(exitBtn);


	auto saveBtn = Button::create();
    saveBtn->setTouchEnabled(true);
    saveBtn->loadTextures("blue_button.png", "blue_button.png", "");
    saveBtn->setPosition(Point(250,15));
    saveBtn->addTouchEventListener(this, toucheventselector(LevelEditor::saveLevel));
	saveBtn->setTitleText("Save");
	saveBtn->setTag(0);
	this->addChild(saveBtn);

	auto brickNomalBtn = Button::create();
    brickNomalBtn->setTouchEnabled(true);
    brickNomalBtn->loadTextures("blue_button.png", "blue_button.png", "");
    brickNomalBtn->setPosition(Point(325, 15));
    brickNomalBtn->addTouchEventListener(this, toucheventselector(LevelEditor::BrickTypeNormal));
	brickNomalBtn->setTitleText("OneHit");
	brickNomalBtn->setTag(0);
	this->addChild(brickNomalBtn);

	auto brickExplodeBtn = Button::create();
    brickExplodeBtn->setTouchEnabled(true);
    brickExplodeBtn->loadTextures("blue_button.png", "blue_button.png", "");
    brickExplodeBtn->setPosition(Point(400, 15));
    brickExplodeBtn->addTouchEventListener(this, toucheventselector(LevelEditor::BrickTypeExplode));
	brickExplodeBtn->setTitleText("Explode");
	brickExplodeBtn->setTag(0);
	this->addChild(brickExplodeBtn);

	auto brickDoubleBtn = Button::create();
    brickDoubleBtn->setTouchEnabled(true);
    brickDoubleBtn->loadTextures("blue_button.png", "blue_button.png", "");
    brickDoubleBtn->setPosition(Point(475, 15));
    brickDoubleBtn->addTouchEventListener(this, toucheventselector(LevelEditor::BrickTypeDoubleHit));
	brickDoubleBtn->setTitleText("DoubleHit");
	brickDoubleBtn->setTag(0);
	this->addChild(brickDoubleBtn);

	auto brickSolidBtn = Button::create();
    brickSolidBtn->setTouchEnabled(true);
    brickSolidBtn->loadTextures("blue_button.png", "blue_button.png", "");
    brickSolidBtn->setPosition(Point(550, 15));
    brickSolidBtn->addTouchEventListener(this, toucheventselector(LevelEditor::BrickTypeSolid));
	brickSolidBtn->setTitleText("Solid");
	brickSolidBtn->setTag(0);
	this->addChild(brickSolidBtn);

	LevelManager::getInstance().loadLevel(m_level);

	//create the bricks
	std::vector<LevelManager::BrickData> brickList = LevelManager::getInstance().m_bricks;
	for(LevelManager::BrickData brickData : brickList)
	{
		Brick* brick = new Brick(brickData.type);
		brick->m_sprite->setPosition(Point(brickData.x, brickData.y));
		this->addChild(brick->m_sprite);
		bricks.push_back(brick);
	}

	m_maxBalls = LevelManager::getInstance().m_maxBalls;

	createLabels();

    return true;
}

bool LevelEditor::onTouchBegan(Touch* touch, Event* event)
{
	Point touchLocation = touch->getLocation();

	for (auto it = bricks.begin() ; it != bricks.end(); ++it)
	{
		if((*it)->m_sprite->getBoundingBox().containsPoint(touchLocation))
		{
			removeChild((*it)->m_sprite);
			bricks.erase(it);
			m_lastBrick = nullptr;
			return false;
		}
	}

	//add a box there!
	auto brick =new Brick(m_brickType);
    brick->m_sprite->setPosition(touchLocation);
    this->addChild(brick->m_sprite);
	bricks.push_back(brick);
	this->m_lastBrick = brick;

	return true;
}
/**
* Exports the current level setup to a string that can later be used!
*/
std::string LevelEditor::exportLevel()
{
	m_maxBalls = atoi(m_ballsTxt->getStringValue().c_str());
	m_oneStarScore = atoi(m_oneStarTxt->getStringValue().c_str());
	m_twoStarScore = atoi(m_twoStarTxt->getStringValue().c_str());
	m_threeStarScore = atoi(m_threeStarTxt->getStringValue().c_str());

	std::string fileName = "level_" + std::to_string(m_level) + ".json";
	std::string path = FileUtils::getInstance()->fullPathForFilename(fileName);

	FILE* pFile = fopen(path.c_str(), "wb");

	//build the json string to save
	std::string output = "{"; //start of json

	//save # of balls
	output.append(" \"balls\" :" + std::to_string(m_maxBalls)); 
	output.append(",");

	output.append(" \"onestar\" :" + std::to_string(m_oneStarScore)); 
	output.append(",");

	output.append(" \"twostar\" :" + std::to_string(m_twoStarScore)); 
	output.append(",");

	output.append(" \"threestar\" :" + std::to_string(m_threeStarScore)); 
	output.append(",");

	output.append("\"bricks\" :[");
	for(Brick* brick : bricks)
	{
		output.append("{");
		output.append(" \"x\":" + std::to_string(brick->m_sprite->getPosition().x) ); 
		output.append(",");
		output.append(" \"y\":" + std::to_string(brick->m_sprite->getPosition().y) );
		output.append(",");
		output.append(" \"type\":" + std::to_string(brick->m_brickType));
		output.append(" },");
	}
	output.pop_back();//remove the last comma...
	output.append("]");

	output.append("}"); //end of json

	fwrite(output.c_str(), sizeof(char) * output.size(), 1, pFile);
	fclose (pFile);
	
	return "";
}

void LevelEditor::exitLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				//go back to start game scene
                Director::getInstance()->replaceScene(HelloWorld::createScene());
                break;
                
            default:
                break;
        }
}

void LevelEditor::saveLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
                log("saving level!");
				exportLevel();
                break;
                
            default:
                break;
        }
}

void LevelEditor::BrickTypeNormal(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				log("BRICK TYPE NORMAL");
                m_brickType = Brick::BrickType::NORMAL;
                break;
                
            default:
                break;
        }
}

void LevelEditor::BrickTypeExplode(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
			case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				log("BRICK TYPE EXPLODE");
				m_brickType = Brick::BrickType::EXPLODING;
                break;
                
            default:
                break;
        }
}

void LevelEditor::BrickTypeDoubleHit(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				log("BRICK TYPE DOUBLE HIT");
				m_brickType = Brick::BrickType::DOUBLEHIT;
                break;
                
            default:
                break;
        }
}

void LevelEditor::BrickTypeSolid(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
           case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				log("BRICK TYPE SOLID");
				m_brickType = Brick::BrickType::SOLID;
                break;
                
            default:
                break;
        }
}


void LevelEditor::createLabels()
{
	using namespace cocos2d::ui;

	//create names for the score and balls left
	TTFConfig label_config_names;
	label_config_names.fontFilePath = "rainfall.otf";
	label_config_names.fontSize = 18;

	/**One Star Score Label */
	auto oneStarLbl = Label::createWithTTF(label_config_names, "One Star Score");
	oneStarLbl->setPositionX(80);
	oneStarLbl->setPositionY(100);
	oneStarLbl->setTextColor(Color4B(204,55,55,255));
	addChild(oneStarLbl);
	
	m_oneStarTxt = cocos2d::ui::TextField::create("","Arial", 18);
	m_oneStarTxt->setText(std::to_string(LevelManager::getInstance().m_oneStarScore));
	m_oneStarTxt->setPosition(Vec2(170,100));
	m_oneStarTxt->setMaxLength(40);
	m_oneStarTxt->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_oneStarTxt->setMaxLengthEnabled(true);
	this->addChild(m_oneStarTxt);
	/********************/

	/**Two Star Score Label */
	auto twoStarLbl = Label::createWithTTF(label_config_names, "Two Star Score");
	twoStarLbl->setPositionX(80);
	twoStarLbl->setPositionY(80);
	twoStarLbl->setTextColor(Color4B(204,55,55,255));
	addChild(twoStarLbl);
	
	m_twoStarTxt = cocos2d::ui::TextField::create("","Arial", 18);
	m_twoStarTxt->setText(std::to_string(LevelManager::getInstance().m_twoStarScore));
	m_twoStarTxt->setPosition(Vec2(170,80));
	m_twoStarTxt->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_twoStarTxt->setMaxLength(40);
	m_twoStarTxt->setMaxLengthEnabled(true);
	this->addChild(m_twoStarTxt);
	/********************/

	/**Three Star Score Label */
	auto threeStarLbl = Label::createWithTTF(label_config_names, "Thr Star Score");
	threeStarLbl->setPositionX(80);
	threeStarLbl->setPositionY(60);
	threeStarLbl->setTextColor(Color4B(204,55,55,255));
	addChild(threeStarLbl);
	
	m_threeStarTxt = cocos2d::ui::TextField::create("","Arial", 18);
	m_threeStarTxt->setText(std::to_string(LevelManager::getInstance().m_threeStarScore));
	m_threeStarTxt->setPosition(Vec2(170,60));
	m_threeStarTxt->setMaxLength(40);
	m_threeStarTxt->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_threeStarTxt->setMaxLengthEnabled(true);
	this->addChild(m_threeStarTxt);
	/********************/

	/**Balls Label */
	auto ballsNameLbl = Label::createWithTTF(label_config_names, "balls");
	ballsNameLbl->setPositionX(60);
	ballsNameLbl->setPositionY(40);
	ballsNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(ballsNameLbl);
	
	m_ballsTxt = cocos2d::ui::TextField::create("","Arial", 18);
	m_ballsTxt->setText(std::to_string(LevelManager::getInstance().m_maxBalls));
	m_ballsTxt->setPosition(Vec2(170,40));
	m_ballsTxt->setMaxLength(2);
	m_ballsTxt->setMaxLengthEnabled(true);
	this->addChild(m_ballsTxt);
	/********************/

}

void LevelEditor::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
}

void LevelEditor::keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (m_lastBrick == nullptr)
		return;

	if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		m_lastBrick->m_sprite->setPositionY(m_lastBrick->m_sprite->getPositionY() + 1);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		m_lastBrick->m_sprite->setPositionY(m_lastBrick->m_sprite->getPositionY() - 1);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		m_lastBrick->m_sprite->setPositionX(m_lastBrick->m_sprite->getPositionX() - 1);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_lastBrick->m_sprite->setPositionX(m_lastBrick->m_sprite->getPositionX() + 1);
	}

}
