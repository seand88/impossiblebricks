#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Brick.h"

USING_NS_CC;

class LevelEditor : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int level);
	static LevelEditor* create(int level);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
	int m_level;
	int m_brickType;
	int m_maxBalls;
	int m_oneStarScore;
	int m_twoStarScore;
	int m_threeStarScore;
	cocos2d::ui::TextField* m_ballsTxt;
	cocos2d::ui::TextField* m_oneStarTxt;
	cocos2d::ui::TextField* m_twoStarTxt;
	cocos2d::ui::TextField* m_threeStarTxt;

	Brick* m_lastBrick;

	Sprite* dropZone;
	std::vector<Brick*> bricks;

	bool onTouchBegan(Touch* touch, Event* event);
	
	std::string exportLevel();
	void saveLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
	void exitLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
	void BrickTypeNormal(Ref *pSender, cocos2d::ui::TouchEventType type);
	void BrickTypeExplode(Ref *pSender, cocos2d::ui::TouchEventType type);
	void BrickTypeDoubleHit(Ref *pSender, cocos2d::ui::TouchEventType type);
	void BrickTypeSolid(Ref *pSender, cocos2d::ui::TouchEventType type);
	void createLabels();

	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

};

#endif // __LEVEL_EDITOR_H__
