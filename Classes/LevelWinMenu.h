#ifndef  _LEVEL_WIN_MENU_H_
#define  _LEVEL_WIN_MENU_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LevelWinMenu : public cocos2d::Layer
{
	public:
		 static LevelWinMenu *create(int ballsRemaining, int currentScore);
		 void nextLevel(Ref *pSender, cocos2d::ui::TouchEventType type);
		 void levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type);
		 void drawStars(int finalScore);

		 int m_ballsRemaining;
		 int m_currentScore;
		 int m_finalScore;

	protected:
		 LevelWinMenu();
		  ~LevelWinMenu();

		  virtual bool init() override;
};

#endif //end _LEVEL_WIN_MENU_H_