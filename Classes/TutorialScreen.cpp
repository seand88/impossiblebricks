#include "TutorialScreen.h"
#include "GameScene.h"
#include "LevelManager.h"
#include "LevelSelectScene.h"

TutorialScreen *TutorialScreen::create(int level)
{
	TutorialScreen *ret = new TutorialScreen();
	ret->m_currentLevel = level;
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool TutorialScreen::init()
{
	using namespace cocos2d::ui;
	using namespace cocos2d;

	Size screenSize = Director::getInstance()->getWinSize();
	setContentSize(screenSize);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	bool hasScreen = true;
	std::string imageName = "";
	//check if we have a tutorial screen for this level
	switch (m_currentLevel)
	{
		case 1:
			imageName = "tutorial/vv_tutorial_screens_drag.png";
			break;

		case 2:
			imageName = "tutorial/vv_tutorial_screens_multiple.png";
			break;

		case 3:
			imageName =  "tutorial/vv_tutorial_screens_blue.png";
			break;

		case 4:
			imageName = "tutorial/vv_tutorial_screens_red.png";
			break;

		case 5:
			imageName = "tutorial/vv_tutorial_screens_white.png";
			break;

		case 6:
			imageName = "tutorial/vv_tutorial_screens_wall.png";
			break;

		default:
			hasScreen = false;
			break;
	}

	if (hasScreen)
	{
		//add the graphics
		auto backgroundGraphic = Sprite::create(imageName);
		backgroundGraphic->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(backgroundGraphic, 0);

		//add the continue button
		auto levelSelectBtn = Button::create();
		levelSelectBtn->setTouchEnabled(true);
		levelSelectBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
		levelSelectBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 4) + Point(0, -50));
		levelSelectBtn->addTouchEventListener(this, toucheventselector(TutorialScreen::continueToLevel));
		levelSelectBtn->setTitleText("Continue");
		levelSelectBtn->setTitleFontName("Arial");
		levelSelectBtn->setTitleFontSize(36);
		levelSelectBtn->setTag(1);
		this->addChild(levelSelectBtn);
	}
	else
	{
		//there is no tutorial screen so just remove it from parent if we have added it
		if (this->getParent() != nullptr)
		{
			this->getParent()->removeChild(this);
		}
	}

	return true;
}

TutorialScreen::TutorialScreen(){
}



void TutorialScreen::continueToLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	switch (type)
	{
		case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
			if (this->getParent() != nullptr)
			{
				this->getParent()->removeChild(this);
			}
			break;
            
        default:
            break;
	}
}


TutorialScreen::~TutorialScreen()
{

}