#include "LevelFailMenu.h"
#include "GameScene.h"
#include "LevelManager.h"
#include "LevelSelectScene.h"

LevelFailMenu *LevelFailMenu::create() 
{
    LevelFailMenu *ret = new LevelFailMenu();
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

bool LevelFailMenu::init()
{
	using namespace cocos2d::ui;
	using namespace cocos2d;

	Size screenSize = Director::getInstance()->getWinSize();
	setContentSize(screenSize);

	auto continueBtn = Button::create();
    continueBtn->setTouchEnabled(true);
    continueBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
    continueBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 2) + Point(0, -50));
    continueBtn->addTouchEventListener(this, toucheventselector(LevelFailMenu::retryLevel));
	continueBtn->setTitleText("Retry?");
	continueBtn->setTitleFontName("Arial");
	continueBtn->setTitleFontSize(30);
	continueBtn->setTag(0);
	this->addChild(continueBtn);

	auto levelSelectBtn = Button::create();
    levelSelectBtn->setTouchEnabled(true);
    levelSelectBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
    levelSelectBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 3) + Point(0, -50));
    levelSelectBtn->addTouchEventListener(this, toucheventselector(LevelFailMenu::levelSelect));
	levelSelectBtn->setTitleText("Back To Level Select");
	levelSelectBtn->setTitleFontName("Arial");
	levelSelectBtn->setTitleFontSize(36);
	levelSelectBtn->setTag(1);
	this->addChild(levelSelectBtn);

	TTFConfig label_config_names;
	label_config_names.fontFilePath = "rainfall.otf";
	label_config_names.fontSize = 28;

	auto scoreNameLbl = Label::createWithTTF(label_config_names, "You Failed The Level");
	scoreNameLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.5f) + Point(0, -50));
	scoreNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(scoreNameLbl);

	return true;
}

LevelFailMenu::LevelFailMenu(){
}

void LevelFailMenu::retryLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
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

void LevelFailMenu::levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type)
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


LevelFailMenu::~LevelFailMenu()
{

}