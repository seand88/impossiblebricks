#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "GameObject.h"
#include "ui/CocosGUI.h"
#include "Warp.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer, public b2ContactListener
{
public:

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int level);
	static GameScene* create(int level);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	int m_level;

    b2World *world;
    float deltaTime;

	std::vector<b2Body*> bodiesToRemove;
	std::vector<cocos2d::Vec2> m_explosions;
	std::vector<b2Body*> m_warpsHit;
	std::vector<GameObject*> m_exploBalls;

	Label* m_scoreLbl;
	Label* m_ballsLbl;
	int m_score;
	int m_maxBalls;
	int m_ballsRemaining;
	int m_hits; //# of hits per ball

	Sprite *dropZone;
    GameObject *ball;
    bool existBall;
    float ballX;
    float ballY;    
    int dragOffsetStartX;
    int dragOffsetEndX;
    int dragOffsetStartY;
    int dragOffsetEndY;    
    float powerMultiplier;
    b2CircleShape m_ballShape;
    b2BodyDef m_ballBodyDef;  

	bool levelComplete;
	bool ballReady;
	bool isTouching;
	ParticleSystemQuad *ballTrail;
    Point m_lastTouchLocation;
    
    Sprite *points[32];

	void addBrick(float x, float y, int type);
	void addWarp(float x, float y);
	void addWall(float w,float h,float px,float py);
	void simulateTrajectory(b2Vec2 coord);
	void moveBall(float x, float y);
	void showFadingHitLabel(float x, float y);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void update(float dt);
	void defineBall();
	void createUI();
	void updateUI();
	void createBrickExplosion(cocos2d::Vec2 position);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//contact listeners
	void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
	void exit(Ref *pSender, cocos2d::ui::TouchEventType type);
	void showLevelComplete(bool won);
};


#endif // __GAME_SCENE_H__