#ifndef __LEVELSELECT_SCENE_H__
#define __LEVELSELECT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class LevelSelect : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool isLevelEditor);
	static LevelSelect* create(bool isLevelEditor);

	bool m_isLevelEditor;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	void touchEvent(Ref *pSender, cocos2d::ui::TouchEventType type);
	void menuItemCallback(Ref *pSender);
	
    
	
};

#endif // __LEVELSELECT_SCENE_H__
