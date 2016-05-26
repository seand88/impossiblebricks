#ifndef  _LEVEL_FAIL_MENU_H_
#define  _LEVEL_FAIL_MENU_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LevelFailMenu : public cocos2d::Layer
{
	public:
		 static LevelFailMenu *create();
		 void retryLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
		 void levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type);

	protected:
		 LevelFailMenu();
		  ~LevelFailMenu();

		  virtual bool init() override;
};

#endif //end _LEVEL_FAIL_MENU_H_