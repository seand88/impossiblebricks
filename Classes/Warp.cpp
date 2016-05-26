#include "Warp.h"

#define SCALE_RATIO 32.0

Warp::Warp() : GameObject(GameObject::ObjectType::WARP) 
{
	m_health = 0;
	
	this->createSprite("warp_circle.png");
	
	auto warpParticle = cocos2d::CCParticleSystemQuad::create("particles/particle_warp.plist");
	warpParticle->setDuration(-1);
	warpParticle->setPositionX(25);
	warpParticle->setPositionY(25);
	this->m_sprite->addChild(warpParticle);
}

void Warp::createPhysicsBody(b2World* world, float xPos, float yPos)
{
	int size = 13;

	b2PolygonShape brickShape;

    brickShape.SetAsBox(size/ SCALE_RATIO,size/ SCALE_RATIO);
    b2FixtureDef brickFixture;
    brickFixture.density=1;
    brickFixture.friction=2;
	brickFixture.isSensor = true;
    brickFixture.restitution=1;
    brickFixture.shape=&brickShape;
	brickFixture.filter.categoryBits = GameObject::CollisionCategory::WARP1;
    b2BodyDef brickBodyDef;

    brickBodyDef.position.Set(xPos/ SCALE_RATIO, yPos/ SCALE_RATIO);
    b2Body *brickBody = world->CreateBody(&brickBodyDef);
	brickBody->SetUserData(this);

    brickBody->CreateFixture(&brickFixture);

	m_physicsBody = brickBody;
	m_hasPhysics = true;
}


void Warp::hit()
{
	if (!isDead())
	{
		m_health--;
	}
}

bool Warp::isDead()
{
	if(m_health <= 0)
		return true;
	else
		return false;
}