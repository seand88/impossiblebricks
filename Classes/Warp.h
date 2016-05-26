#ifndef  _WARP_H_
#define  _WARP_H_

#include "GameObject.h"

/**
* Brick Game Object, Can Have Multiple Types
*
*/
class Warp : public GameObject
{
	public:
		int m_health; //health for how many times can the warp be used

		void createPhysicsBody(b2World* world, float xPos, float yPos);

		Warp();
		bool isDead();
		void hit();

};

#endif //end _WARP_H_