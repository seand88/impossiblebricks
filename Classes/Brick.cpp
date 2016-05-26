#include "Brick.h"

#define SCALE_RATIO 32.0

Brick::Brick(const int brickType) : GameObject(GameObject::ObjectType::BRICK) 
{
	this->m_brickType = brickType;
	m_health = 0;
	
	if (brickType == Brick::BrickType::DOUBLEHIT)
	{
		//if double hit change to blue
		this->createSprite("vertvel_blueBlock.png");
		m_health = 2;
	}
	else if (brickType == Brick::BrickType::EXPLODING)
	{
		this->createSprite("vertvel_redBlock.png");
	}
	else if (brickType == Brick::BrickType::SOLID)
	{
		//solid bricks withstand 5 hits... 
		m_health = 5; 
		this->createSprite("vertvel_whiteWall.png");
	}
	else 
	{
		//default brick
		this->createSprite("vertvel_yellowBlock.png");
	}

	//for some reason drawing over debug draw, if you want to see debug just set this opacity
	//this->m_sprite->setOpacity(142);
}

void Brick::createPhysicsBody(b2World* world, float xPos, float yPos)
{
	int size = 13;
	if (m_brickType == Brick::BrickType::SOLID)
	{
		size = 7; //wall bricks are smaller
	}

	b2PolygonShape brickShape;

    brickShape.SetAsBox(size/ SCALE_RATIO,size/ SCALE_RATIO);
    b2FixtureDef brickFixture;
    brickFixture.density=1;
    brickFixture.friction=2;
    brickFixture.restitution=1;
    brickFixture.shape=&brickShape;
	brickFixture.filter.categoryBits = GameObject::CollisionCategory::BRICK1;
    b2BodyDef brickBodyDef;

    brickBodyDef.position.Set(xPos/ SCALE_RATIO, yPos/ SCALE_RATIO);
    b2Body *brickBody = world->CreateBody(&brickBodyDef);
	brickBody->SetUserData(this);

    brickBody->CreateFixture(&brickFixture);

	m_physicsBody = brickBody;
	m_hasPhysics = true;

}


void Brick::hit()
{
	if (!isDead())
	{
		m_health--;

		if (m_brickType == Brick::BrickType::DOUBLEHIT)
		{
			//change back to yellow
            m_sprite->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("vertvel_yellowBlock.png"));
		}
	}
}

bool Brick::isDead()
{
	if(m_health <= 0)
		return true;
	else
		return false;
}