#include "LevelWinMenu.h"
#include "GameScene.h"
#include "LevelManager.h"
#include "LevelSelectScene.h"
#include "SaveManager.h"


LevelWinMenu *LevelWinMenu::create(int ballsRemaining, int currentScore) 
{
    LevelWinMenu *ret = new LevelWinMenu();
	ret->m_ballsRemaining = ballsRemaining;
	ret->m_currentScore = currentScore;

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

bool LevelWinMenu::init()
{
	using namespace cocos2d::ui;
	using namespace cocos2d;

    Size screenSize = Director::getInstance()->getWinSize();
	setContentSize(screenSize);

	auto winParticle = CCParticleSystemQuad::create("particles/particle_win.plist");
	winParticle->setDuration(-1);
	winParticle->setPosition(Point(screenSize.width / 2, -125) );
	this->addChild(winParticle);

	int nextLevel = LevelManager::getInstance().m_lastLevel + 1;
	if (nextLevel <= LevelManager::getInstance().totalLevels) //only show continue button if there is a max level!
	{
		auto continueBtn = Button::create();
		continueBtn->setTouchEnabled(true);
		continueBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
		continueBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 2) + Point(0, -50));
		continueBtn->addTouchEventListener(this, toucheventselector(LevelWinMenu::nextLevel));
		continueBtn->setTitleText("Continue?");
		continueBtn->setTitleFontName("Arial");
		continueBtn->setTitleFontSize(30);
		continueBtn->setTag(0);
		this->addChild(continueBtn);
	}

	auto levelSelectBtn = Button::create();
    levelSelectBtn->setTouchEnabled(true);
    levelSelectBtn->loadTextures("vertvel_playBtn.png", "vertvel_playBtn.png", "");
    levelSelectBtn->setPosition(Point(screenSize.width / 2, screenSize.height / 3) + Point(0, -50));
    levelSelectBtn->addTouchEventListener(this, toucheventselector(LevelWinMenu::levelSelect));
	levelSelectBtn->setTitleText("Back To Level Select");
	levelSelectBtn->setTitleFontName("Arial");
	levelSelectBtn->setTitleFontSize(36);
	levelSelectBtn->setTag(1);
	this->addChild(levelSelectBtn);

	TTFConfig label_config_names;
	label_config_names.fontFilePath = "rainfall.otf";
	label_config_names.fontSize = 28;

	TTFConfig label_config;
	label_config.fontFilePath = "draco.ttf";
	label_config.fontSize = 24;

	auto completeLbl = Label::createWithTTF(label_config_names, "You Completed The Level");
	completeLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.2f));
	completeLbl->setTextColor(Color4B(204,55,55,255));
	addChild(completeLbl);

	auto scoreNameLbl = Label::createWithTTF(label_config, "score: " + std::to_string(m_currentScore));
	scoreNameLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.3f));
	scoreNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(scoreNameLbl);

	auto ballsNameLbl = Label::createWithTTF(label_config, "balls remaining: " + std::to_string(m_ballsRemaining));
	ballsNameLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.4f));
	ballsNameLbl->setTextColor(Color4B(204,55,55,255));
	addChild(ballsNameLbl);

	//calculate final score
	m_finalScore = m_currentScore + (m_ballsRemaining * 1000);

	auto finalScoreLbl = Label::createWithTTF(label_config, "final score: " + std::to_string(m_finalScore));
	finalScoreLbl->setPosition(Point(screenSize.width / 2, screenSize.height / 1.5f));
	finalScoreLbl->setTextColor(Color4B(204,55,55,255));
	addChild(finalScoreLbl);

	drawStars(m_finalScore);

	//save the current score
	SaveManager::updateScore(LevelManager::getInstance().m_lastLevel, m_finalScore);

	//unlock the next level
	SaveManager::unlockNextLevel();

	return true;
}

void LevelWinMenu::drawStars(int finalScore)
{
	Size screenSize = Director::getInstance()->getWinSize();

	//save the number of stars they get!
	int numStars = 0;
	if (finalScore >= LevelManager::getInstance().m_oneStarScore)
			numStars = 1;
	if (finalScore >= LevelManager::getInstance().m_twoStarScore)
			numStars = 2;
	if (finalScore >= LevelManager::getInstance().m_threeStarScore)
			numStars = 3;

	int startX = screenSize.width / 2.5f;
	int startY = 550;
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
		star->setOpacity(0);
		addChild(star);
		startX += 50;

		//fade the star in
		FadeIn *fade = FadeIn::create((i * 1.0f));
		star->runAction(fade);
	}

}

LevelWinMenu::LevelWinMenu(){
}

void LevelWinMenu::nextLevel(Ref *pSender, cocos2d::ui::TouchEventType type)
{
        switch (type)
        {
            case cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN:
				//load last level played
				Director::getInstance()->replaceScene(GameScene::createScene(LevelManager::getInstance().m_lastLevel + 1));
                break;
                
            default:
                break;
        }
}

void LevelWinMenu::levelSelect(Ref *pSender, cocos2d::ui::TouchEventType type)
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


LevelWinMenu::~LevelWinMenu()
{

}