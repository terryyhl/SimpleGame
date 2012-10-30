#include "AppDelegate.h"

#include "cocos2d.h"
#include "HelloWorldScene.h"

#include <android/log.h> 
#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) 

USING_NS_CC;

AppDelegate::AppDelegate()
{
   LOGD("Application -->AppDelegate::AppDelegate");
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
   LOGD("Application -->AppDelegate::applicationDidFinishLaunching"); 
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    //get Resource sd root png
    CCFileUtils::sharedFileUtils()->setResourceDirectory("sd");
	//set Resolution size width=480 height =320 && not border
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(480, 320, kResolutionNoBorder);
	
    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
    // pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    LOGD("Application -->AppDelegate::applicationDidEnterBackground");
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    LOGD("Application -->AppDelegate::applicationWillEnterForeground");
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
