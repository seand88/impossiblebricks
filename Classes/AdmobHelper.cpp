#include "AdmobHelper.h"
#include "cocos2d.h"

bool AdmobHelper::isAdShowing = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void AdmobHelper::initBannerAd()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "initBannerAd", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);        
    }
}

void AdmobHelper::showAd(bool show)
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "(Z)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, show);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = show;
	}
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "AdMob.h"

void AdmobHelper::initBannerAd()
{
    Social::AdMob::initBannerAd();
}

void AdmobHelper::showAd(bool show)
{
    Social::AdMob::showAd(show);
    isAdShowing = show;
}

#else

void AdmobHelper::initBannerAd()
{
	cocos2d::log("initBannerAd() called");
}

void AdmobHelper::showAd(bool show)
{
	cocos2d::log("showAd() called");
	isAdShowing = show;
}

#endif
