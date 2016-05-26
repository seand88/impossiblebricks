#ifndef  _BRICK_H_
#define  _BRICK_H_

#include "GameObject.h"

/**
* Brick Game Object, Can Have Multiple Types
*
*/
class Brick : public GameObject
{
	public:
		int m_brickType;
		int m_health; //health for how many times need to hit the brick

		enum BrickType
		{
			NORMAL,
			EXPLODING,
			DOUBLEHIT,
			SOLID
		};

		void createPhysicsBody(b2World* world, float xPos, float yPos);

		Brick(const int brickType);
		bool isDead();
		void hit();

};

#endif //end __BRICK_H_