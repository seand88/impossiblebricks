#include "GameObject.h"
#include "cocos2d.h"

#define SCALE_RATIO 32.0


//create game object just with type
GameObject::GameObject(const int type)
{ 
	this->m_objectType = type;
	//no sprite or physics
	this->m_hasSprite = false;
	this->m_hasPhysics = false;
	this->m_alive = true;
}

void GameObject::createSprite(const std::string& filename)
{
	this->m_hasSprite = true;
	this->m_sprite = cocos2d::Sprite::create(filename);
}


//create game object with type and filename, initialize the sprite
GameObject::GameObject(const int type, const std::string& filename)
{
	this->m_objectType = type;
	this->m_hasSprite = true;
	this->m_hasPhysics = false;
	this->m_sprite = cocos2d::Sprite::create(filename);
}

//runs on the specified time frame
void GameObject::update()
{
	if (this->m_hasPhysics && this->m_hasSprite)
	{
		//sync position
		m_sprite->setPosition(cocos2d::Vec2(m_physicsBody->GetPosition().x * SCALE_RATIO,m_physicsBody->GetPosition().y * SCALE_RATIO));
		m_sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(m_physicsBody->GetAngle()));
	}
}

GameObject::~GameObject()
{
	if (this->m_hasSprite){
		this->m_sprite->release();
	}

	if(this->m_hasPhysics){
		//TODO: release the physics body
	}
}
