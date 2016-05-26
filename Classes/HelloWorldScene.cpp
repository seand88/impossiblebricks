#include "HelloWorldScene.h"
#include "B2DebugDrawLayer.h"
#include "LevelSelectScene.h"
#include "GameScene.h"
#include "LevelManager.h"
#include "LevelEditor.h"
#include "AdmobHelper.h"
#include "AdMob.h"
USING_NS_CC;

#define SCALE_RATIO 32.0

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
	Size screenSize = Director::getInstance()->getWinSize();

	using namespace cocos2d::ui;

	//add the background particle
	ParticleSystemQuad *bgParticle = ParticleSystemQuad::create("particles/game_bg.plist");
	this->addChild(bgParticle);

	auto logoGraphic = Sprite::create("logo.png");
	logoGraphic->setPosition(visibleSize.width/2, visibleSize.height/1.3);
	this->addChild(logoGraphic, 0);

	auto playBtn = Button::create();
    playBtn->setTouchEnabled(true);
    playBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
    playBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 2) + Point(0, -50));
    playBtn->addTouchEventListener(this, toucheventselector(HelloWorld::playGame));
	playBtn->setTitleText("Play");
	playBtn->setTitleFontName("Arial");
	playBtn->setTitleFontSize(36);
	playBtn->setTag(0);
	this->addChild(playBtn);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto editBtn = Button::create();
    editBtn->setTouchEnabled(true);
    editBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
    editBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 3) + Point(0, -50));
    editBtn->addTouchEventListener(this, toucheventselector(HelloWorld::openLevelEditor));
	editBtn->setTitleText("LevelEditor");
	editBtn->setTitleFontName("Arial");
	editBtn->setTitleFontSize(36);
	editBtn->setTag(1);
	this->addChild(editBtn);
#endif

    return true;
}

void HelloWorld::openLevelEditor(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
		case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
                log("UI BUTTON CLICK!");
				// create a scene. it's an autorelease object
				Director::getInstance()->replaceScene(LevelSelect::createScene(true));
                break;
                
            default:
                break;
        }
}

void HelloWorld::playGame(Ref *pSender, cocos2d::ui::TouchEventType type)
    {
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
                log("UI BUTTON CLICK!");
				// create a scene. it's an autorelease object
				Director::getInstance()->replaceScene(LevelSelect::createScene(false));
                break;
                
            default:
                break;
        }
    }

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
