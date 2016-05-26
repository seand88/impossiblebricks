#ifndef  _LEVEL_PAUSE_MENU_H_
#define  _LEVEL_PAUSE_MENU_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LevelPauseMenu : public cocos2d::Layer
{
public:
	static LevelPauseMenu *create();
	void retryLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
	void levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type);
	void continueLevel(Ref *pSender, cocos2d::ui::TouchEventType type);

protected:
	LevelPauseMenu();
	~LevelPauseMenu();

	virtual bool init() override;
};

#endif //end _LEVEL_PAUSE_MENU_H_