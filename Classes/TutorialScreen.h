#ifndef _TUTORIAL_SCREEN_H_
#define _TUTORIAL_SCREEN_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class TutorialScreen : public cocos2d::Layer
{
public:
	static TutorialScreen *create(int level);
	void continueToLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
	int m_currentLevel; //level tutorial screen was created for

protected:
	TutorialScreen();
	~TutorialScreen();

	virtual bool init() override;
};

#endif //end _TUTORIAL_SCREEN_H_