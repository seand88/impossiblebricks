#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SaveManager.h"
#include "AdmobHelper.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}


//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}



// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}



typedef struct tagResource
    {
        cocos2d::Size size;
        char directory[100];
    }Resource;

    static Resource smallResource  =  { cocos2d::Size(480, 320),   "iphone" };
    static Resource mediumResource =  { cocos2d::Size(1024, 768),  "ipad"   };
    static Resource largeResource  =  { cocos2d::Size(2048, 1536), "ipadhd" };

	//right now only using standard definition resource
	static Resource sdResource  =  { cocos2d::Size(640, 960),   "sd" };


bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview) 
	{
		glview = GLViewImpl::create("Vertical Velocity");
		director->setOpenGLView(glview);
    }

	//adjust window when developing on windows... removes a lot of black space we dont need
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		glview->setFrameSize(640,960);
		// turn on display FPS
		//director->setDisplayStats(true);
	#endif
	
	glview->setDesignResolutionSize(640, 960, ResolutionPolicy::SHOW_ALL);

    FileUtils *fileUtils = FileUtils::getInstance();
	std::vector<std::string> searchPaths = fileUtils->getSearchPaths(); 

	/////////// figure out resolution size
	//right now just using standard size 640 x 960 (standard definition)
	searchPaths.insert(searchPaths.begin(), sdResource.directory);
	//////////////////////////////

	searchPaths.insert(searchPaths.begin(), "particles"); //add particles to search path
	searchPaths.insert(searchPaths.begin(), "fonts"); //add fonts to search path
	fileUtils->setSearchPaths(searchPaths);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	//check for the save file, 
	SaveManager::init();
	
	register_all_packages();
	

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
 
    // run
    director->runWithScene(scene);
 
    //AdmobHelper::initBannerAd();  

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
