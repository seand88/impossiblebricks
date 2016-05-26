#include "LevelPauseMenu.h"
#include "GameScene.h"
#include "LevelManager.h"
#include "LevelSelectScene.h"

LevelPauseMenu *LevelPauseMenu::create()
{
	LevelPauseMenu *ret = new LevelPauseMenu();
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

bool LevelPauseMenu::init()
{
	using namespace cocos2d::ui;
	using namespace cocos2d;

	Size screenSize = Director::getInstance()->getWinSize();
	setContentSize(screenSize);

	auto levelContinueBtn = Button::create();
	levelContinueBtn->setTouchEnabled(true);
	levelContinueBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
	levelContinueBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 2) + Point(0, -50));
	levelContinueBtn->addTouchEventListener(this, toucheventselector(LevelPauseMenu::continueLevel));
	levelContinueBtn->setTitleText("Continue");
	levelContinueBtn->setTitleFontName("Arial");
	levelContinueBtn->setTitleFontSize(36);
	levelContinueBtn->setTag(2);
	this->addChild(levelContinueBtn);

	auto levelSelectBtn = Button::create();
	levelSelectBtn->setTouchEnabled(true);
	levelSelectBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
	levelSelectBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 3) + Point(0, -50));
	levelSelectBtn->addTouchEventListener(this, toucheventselector(LevelPauseMenu::levelSelect));
	levelSelectBtn->setTitleText("Back To Level Select");
	levelSelectBtn->setTitleFontName("Arial");
	levelSelectBtn->setTitleFontSize(36);
	levelSelectBtn->setTag(1);
	this->addChild(levelSelectBtn);

	auto continueBtn = Button::create();
	continueBtn->setTouchEnabled(true);
	continueBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
	continueBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 5) + Point(0, -50));
	continueBtn->addTouchEventListener(this, toucheventselector(LevelPauseMenu::retryLevel));
	continueBtn->setTitleText("Restart?");
	continueBtn->setTitleFontName("Arial");
	continueBtn->setTitleFontSize(30);
	continueBtn->setTag(0);
	this->addChild(continueBtn);

	TTFConfig label_config_names;
	label_config_names.fontFilePath = "rainfall.otf";
	label_config_names.fontSize = 28;

	auto scoreNameLbl = Label::createWithTTF(label_config_names, "Paused");
	scoreNameLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.5f) + Point(0, -50));
	scoreNameLbl->setTextColor(Color4B(204, 55, 55, 255));
	addChild(scoreNameLbl);

	return true;
}

LevelPauseMenu::LevelPauseMenu(){
}

void LevelPauseMenu::continueLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
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

void LevelPauseMenu::retryLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	switch (type)
	{
        case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
            //load last level played
            Director::getInstance()->replaceScene(GameScene::createScene(LevelManager::getInstance().m_lastLevel));
            break;
            
        default:
            break;
	}
}

void LevelPauseMenu::levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	switch (type)
	{
        case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
            Director::getInstance()->replaceScene(LevelSelect::createScene(false));
            break;
            
        default:
            break;
	}
}


LevelPauseMenu::~LevelPauseMenu()
{

}