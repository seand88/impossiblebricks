#include "HelloWorldScene.h"
#include "B2DebugDrawLayer.h"
#include "LevelSelectScene.h"
#include "GameScene.h"
#include "SlidingMenu.h"
#include "LevelManager.h"
#include "LevelEditor.h"
#include "SaveManager.h"

USING_NS_CC;

LevelSelect* LevelSelect::create(bool isLevelEditor)
{ 
	LevelSelect *pRet = new LevelSelect(); 
	pRet->m_isLevelEditor = isLevelEditor;

	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease();
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

Scene* LevelSelect::createScene(bool isLevelEditor)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LevelSelect::create(isLevelEditor);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelSelect::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    log("INIT LEVEL SELECT");
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin     = Director::getInstance()->getVisibleOrigin();
	Size screenSize  = Director::getInstance()->getWinSize();

	TTFConfig label_config;
	label_config.fontFilePath = "absender1.ttf";
	label_config.fontSize = 45;

	TTFConfig label_config_score;
	label_config_score.fontFilePath = "draco.ttf";
	label_config_score.fontSize = 28;

	MenuItemsArray items = MenuItemsArray();//in SlidingMenuGrid: typedef Vector<MenuItem*> MenuItemsArray;
	for(int i=1; i<=30; i++)
	{
		MenuItemImage *menuItem;
		int levelScore = 0;
		//if id exists in the savemanager, then its not locked, otherwise we lock it.
		if (SaveManager::levelExists(i))
		{
			levelScore = SaveManager::getLevel(i)->score;
			int numStars = SaveManager::getLevel(i)->stars;
			menuItem = MenuItemImage::create("vertvel_levelselectBtn.png", "vertvel_levelselectBtn.png", CC_CALLBACK_1(LevelSelect::menuItemCallback, this));

			auto scoreNameLbl = Label::createWithTTF(label_config_score, std::to_string(levelScore));
			menuItem->addChild(scoreNameLbl);
			scoreNameLbl->setPosition(150, 100);

			int startX = 100;
			int startY = 50;
			//need to add the stars!
			for (int i=1; i <= 3; i++)
			{
				cocos2d::Sprite* star;
				if (numStars >= i){
					star = cocos2d::Sprite::create("vertvel_starIcon.png");
				}
				else{
					star = cocos2d::Sprite::create("vertvel_emptystarIcon.png");
				}

				star->setPosition(startX, startY);
				menuItem->addChild(star);
				startX += 50;
			}
		}
		else //level is locked
		{
			menuItem = MenuItemImage::create("vertvel_levelselectBtn_lock.png", "vertvel_levelselectBtn_lock.png");
		}

		menuItem->setTag(i);

		auto levelLabel = Label::createWithTTF( label_config, std::to_string(i),TextHAlignment::CENTER,visibleSize.width);
		levelLabel->setTextColor(Color4B(204,55,55,255));
		menuItem->addChild(levelLabel);
		levelLabel->setPosition(45,75);

		items.pushBack(menuItem);
    }
	
	auto arrowGraphic  = Sprite::create("vv_nav_arrows.png");
    auto gridMenuLayer = SlidingMenuGrid::createWithArray(&items, 2, 5, Point(visibleSize.width/2,visibleSize.height/2),Point(300,140));
	auto label1 = Label::createWithTTF( label_config, "Part One", TextHAlignment::CENTER, visibleSize.width);
    auto label2 = Label::createWithTTF( label_config, "Part Two", TextHAlignment::CENTER, visibleSize.width);
	auto label3 = Label::createWithTTF( label_config, "Part Three", TextHAlignment::CENTER, visibleSize.width);
    gridMenuLayer->setPageLabel(1,label1);
    gridMenuLayer->setPageLabel(2,label2);
	gridMenuLayer->setPageLabel(3,label3);
	gridMenuLayer->addChild(arrowGraphic);
    addChild(gridMenuLayer, 2, 100);
	arrowGraphic->setPosition(Vec2(320, 100));

    return true;
}


void LevelSelect::menuItemCallback(Ref *pSender)
{
    //CCLOG ("Item CLICKED: %d",  ((CCMenuItemSprite*)pSender)->getTag());

	int levelClicked = ((MenuItemImage*)pSender)->getTag();

	if (m_isLevelEditor)
	{
		//switch to the level editor with the current level loaded
		auto scene = LevelEditor::createScene(levelClicked);
		// run
		Director::getInstance()->replaceScene(scene);
	}
	else
	{
		//switch to the game scene for the current level
		auto scene = GameScene::createScene(levelClicked);
		// run
		Director::getInstance()->replaceScene(scene);
	}
}

