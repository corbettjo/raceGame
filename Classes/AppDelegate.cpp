#include "AppDelegate.h"
#include "MainMenu.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) 
	{
		glview = GLViewImpl::create("Race Game");
		glview->setFrameSize(960, 640);
		director->setOpenGLView(glview);
	}

	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60);

	MainMenu scene;
	director->runWithScene(TransitionFade::create(1.0f, scene.createScene()));

	return true;
}

void AppDelegate::applicationDidEnterBackground() 
{
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() 
{
	Director::getInstance()->startAnimation();
}
