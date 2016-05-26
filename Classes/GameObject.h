#ifndef  _GAME_OBJECT_H_
#define  _GAME_OBJECT_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class GameObject 
{
	public:
		int  m_objectType;
		bool m_hasPhysics;
		bool m_hasSprite;
		bool m_alive;

		b2Body* m_physicsBody;
		cocos2d::Sprite* m_sprite;

		enum ObjectType
		{
			BALL,
			BRICK,
			WALL,
			PLATFORM,
			WARP
		};

		enum CollisionCategory {
			BALL1  =	0x0001,
			WALL1 =	0x0002,
			BRICK1 =	0x0004,
			WARP1   =    0x0008
		};

		void createSprite(const std::string& filename);

		GameObject(const int type, const std::string& filename);
		GameObject(const int type);
		 ~GameObject();

		void update();
};

#endif //end _GAME_OBJECT_H_