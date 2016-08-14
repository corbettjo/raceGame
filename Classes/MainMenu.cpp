#include "globals.h"
#include "MainMenu.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h" 
#include <iostream>
#include <fstream>  
#include <limits>
#include <string>

using namespace std;

int MainMenu::level;


Scene* MainMenu::createScene()
{
	auto scene = Scene::create();

	auto layer = MainMenu::create();

	scene->addChild(layer);

	return scene;
}

bool MainMenu::init()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("MenuMusic.wav", true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("choseLevel.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("click.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("back.wav");
	
	if (!Layer::init()) //If the layer does not initialize
	{
		return false;
	}
	 
	startScreen_sprite = Sprite::create("startScreen.png"); //Start screen 
	startScreen_sprite->setScale(1.0f);
	startScreen_sprite->setPosition(480, 320);
	this->addChild(startScreen_sprite, 0, "startScreen_sprite");

	this->schedule(schedule_selector(MainMenu::updateScene)); 

	return true;
}

void MainMenu::updateScene(float delta) //Gets called 60 times every second; Essentially main method
{
	if (std::find(heldKeys.begin(), heldKeys.end(), ENTER) != heldKeys.end())
	{
		this->unscheduleAllSelectors(); //Stops the scheduled update method
		loadTopSelectMenu(); //Loads the main menu
		
		startScreen_sprite->runAction(FadeOut::create(0.5f));
		this->removeChildByName("startScreen_sprite");
	}
}

void MainMenu::loadTopSelectMenu()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav"); //Click sound effect

	//LOAD MENU BACKGROUND
	topMenuBase_sprite = Sprite::create("topMenuBase.png"); //Sprite for the menu background
	topMenuBase_sprite->setScale(1.0f);
	topMenuBase_sprite->setPosition(480, 320);
	topMenuBase_sprite->setOpacity(0);
	
	this->addChild(topMenuBase_sprite, 0, "topMenuBase");
	topMenuBase_sprite->runAction(FadeIn::create(0.5f));

	//CREATES BUTTONS FOR THE MENU
	levelSelectButton = MenuItemImage::create("selectLevelButton.png", "selectLevelButtonClicked.png", this, menu_selector(MainMenu::loadLevelSelectMenu));
	levelSelectButton->setPosition(0,100); //With respect to the menu's psotion (480,320)
	levelSelectButton->setScale(1.0f);

	controlsButton = MenuItemImage::create("controlsButton.png", "controlsButtonClickedx.png", this, menu_selector(MainMenu::loadControlsMenu));
	controlsButton->setPosition(0, 0); //With respect to the menu's psotion (480,320)
	controlsButton->setScale(1.0f);

	aboutButton = MenuItemImage::create("aboutButton.png", "aboutButtonClicked.png", this, menu_selector(MainMenu::loadAboutMenu));
	aboutButton->setPosition(0,-100); //With respect to the menu's psotion (480,320)
	aboutButton->setScale(1.0f);

	quitBigButton = MenuItemImage::create("quitBigButton.png", "quitBigButtonClicked.png", this, menu_selector(MainMenu::menuCloseCallback));
	quitBigButton->setPosition(0, -200); //With respect to the menu's psotion (480,320)
	quitBigButton->setScale(1.0f);

	//CREATES VECTOR TO HOLD MENU ITEMS
	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(levelSelectButton);
	vec.pushBack(controlsButton);
	vec.pushBack(aboutButton);
	vec.pushBack(quitBigButton);

	//CREATES AND LOADS MENU
	cocos2d::Menu* topMenu = Menu::createWithArray(vec); 
	topMenu->setScale(1.0f);
	topMenu->setOpacity(0);
	
	this->addChild(topMenu, 1, "topMenu");
	topMenu->runAction(FadeIn::create(0.5f));
}

void MainMenu::loadLevelSelectMenu(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");

	//FADES OUT AND REMOVES SPRITES FROM PREVIOUS LEVEL OBJECTS
	topMenuBase_sprite->runAction(FadeOut::create(0.5f));
	this->removeChildByName("topMenuBase");
	levelSelectButton->runAction(FadeOut::create(0.5f));
	this->removeChildByName("topMenu");

	//LOADS MENU BACKGROUND
	levelSelectMenuBase_sprite = Sprite::create("selectLevelMenuBase.png");
	levelSelectMenuBase_sprite->setScale(1.0f);
	levelSelectMenuBase_sprite->setPosition(480,320);
	levelSelectMenuBase_sprite->setOpacity(0);
	
	this->addChild(levelSelectMenuBase_sprite, 0, "levelSelectMenuBase");
	levelSelectMenuBase_sprite->runAction(FadeIn::create(0.5f));

	//CREATES BUTTONS FOR THE MENU
	levelEasyButton = MenuItemImage::create("easyButton.png", "easyButtonClicked.png", this, menu_selector(MainMenu::loadEasyLevel));
	levelEasyButton->setPosition(-320, 0);
	levelEasyButton->setScale(1.0f);

	levelMediumButton = MenuItemImage::create("mediumButton.png", "mediumButtonClicked.png", this, menu_selector(MainMenu::loadMediumLevel));
	levelMediumButton->setPosition(0,0);
	levelMediumButton->setScale(1.0f);

	levelHardButton = MenuItemImage::create("hardButton.png", "hardButtonClicked.png", this, menu_selector(MainMenu::loadHardLevel));
	levelHardButton->setPosition(320, 0);
	levelHardButton->setScale(1.0f);

	backButton = MenuItemImage::create("backButton.png", "backButtonClicked.png", this, menu_selector(MainMenu::levelSelectBack));
	backButton->setPosition(0, -240);
	backButton->setScale(1.0f);

	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(levelEasyButton);
	vec.pushBack(levelMediumButton);
	vec.pushBack(levelHardButton);
	vec.pushBack(backButton);

	//Puts the buttons onto the scene
	cocos2d::Menu* levelSelectMenu = Menu::createWithArray(vec);
	levelSelectMenu->setScale(1.0f);
	levelSelectMenu->setOpacity(0);
	this->addChild(levelSelectMenu, 1, "levelSelectMenu");
	levelSelectMenu->runAction(FadeIn::create(0.5f));	
}

void MainMenu::loadAboutMenu(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
	
	//FADES OUT AND REMOVES THE MAIN MENU BUTTONS
	topMenuBase_sprite->runAction(FadeOut::create(0.5f));
	levelSelectButton->runAction(FadeOut::create(0.5f));
	controlsButton->runAction(FadeOut::create(0.5f));
	aboutButton->runAction(FadeOut::create(0.5f));
	quitBigButton->runAction(FadeOut::create(0.5f));
	this->removeChildByName("topMenuBase");
	this->removeChildByName("topMenu");

	//LOADS AND FADES IN MENU BACKGROUND
	aboutMenuBase_sprite = Sprite::create("aboutMenuBase.png");
	aboutMenuBase_sprite->setScale(1.0f);
	aboutMenuBase_sprite->setPosition(480, 320);
	aboutMenuBase_sprite->setOpacity(0);
	this->addChild(aboutMenuBase_sprite, 0, "aboutMenuBase");
	aboutMenuBase_sprite->runAction(FadeIn::create(0.5f));

	//CREATES BACK BUTTON
	backButton = MenuItemImage::create("backButton.png", "backButtonClicked.png", this, menu_selector(MainMenu::aboutBack));
	backButton->setPosition(0, -240);
	backButton->setScale(1.0f);

	//CREATES VECTOR TO HOLD MENU ITEMS
	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(backButton);

	//CREATES AND LOADS ABOUT MENU
	cocos2d::Menu* aboutMenu = Menu::createWithArray(vec);
	aboutMenu->setScale(1.0f);
	aboutMenu->setOpacity(0);
	this->addChild(aboutMenu, 1, "aboutMenu");
	aboutMenu->runAction(FadeIn::create(0.5f));
}

void MainMenu::loadControlsMenu(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
	
	//FADES OUT AND REMOVES THE MAIN MENU BUTTONS
	topMenuBase_sprite->runAction(FadeOut::create(0.5f));
	levelSelectButton->runAction(FadeOut::create(0.5f));
	controlsButton->runAction(FadeOut::create(0.5f));
	aboutButton->runAction(FadeOut::create(0.5f));
	quitBigButton->runAction(FadeOut::create(0.5f));
	this->removeChildByName("topMenuBase");
	this->removeChildByName("topMenu");

	//LOADS AND FADES IN MENU BACKGROUND
	controlsMenuBase_sprite = Sprite::create("controlsMenuBase.png");
	controlsMenuBase_sprite->setScale(1.0f);
	controlsMenuBase_sprite->setPosition(480, 320);
	controlsMenuBase_sprite->setOpacity(0);
	this->addChild(controlsMenuBase_sprite, 0, "controlsMenuBase");
	controlsMenuBase_sprite->runAction(FadeIn::create(0.5f));

	//CREATES BACK BUTTON
	backButton = MenuItemImage::create("backButton.png", "backButtonClicked.png", this, menu_selector(MainMenu::controlsBack));
	backButton->setPosition(0, -240);
	backButton->setScale(1.0f);

	//CREATES VECTOR TO HOLD MENU ITEMS
	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(backButton);

	//CREATES AND LOADS ABOUT MENU
	cocos2d::Menu* controlsMenu = Menu::createWithArray(vec);
	controlsMenu->setScale(1.0f);
	controlsMenu->setOpacity(0);
	this->addChild(controlsMenu, 1, "controlsMenu");
	controlsMenu->runAction(FadeIn::create(0.5f));
}

void MainMenu::levelSelectBack(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("back.wav");
	
	//FADES OUT SELECT LEVEL BUTTONS
	levelEasyButton->runAction(FadeOut::create(0.5f));
	levelMediumButton->runAction(FadeOut::create(0.5f));
	levelHardButton->runAction(FadeOut::create(0.5f));
	backButton->runAction(FadeOut::create(0.5f));

	loadTopSelectMenu();

	this->removeChildByName("levelSelectMenuBase");
	this->removeChildByName("levelSelectMenu");

}

void MainMenu::aboutBack(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("back.wav");
	
	aboutMenuBase_sprite->runAction(FadeOut::create(0.5f));
	backButton->runAction(FadeOut::create(0.5f));
	
	loadTopSelectMenu();

	this->removeChildByName("aboutMenuBase");
	this->removeChildByName("aboutMenu");

}

void MainMenu::controlsBack(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("back.wav");
	
	controlsMenuBase_sprite->runAction(FadeOut::create(0.5f));
	backButton->runAction(FadeOut::create(0.5f));

	loadTopSelectMenu();

	this->removeChildByName("controlsMenuBase");
	this->removeChildByName("controlsMenu");

}

void MainMenu::loadEasyLevel(Ref *psender)
{
	level = 1;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choseLevel.wav");
	Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(1.10f, GameScene::createScene()));
}

void MainMenu::loadMediumLevel(Ref *psender)
{
	level = 2;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choseLevel.wav");
	Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(1.10f, GameScene::createScene()));
}

void MainMenu::loadHardLevel(Ref *psender)
{
	level = 3;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choseLevel.wav");
	Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(1.10f, GameScene::createScene()));
}

void MainMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()){
		heldKeys.push_back(keyCode);
	}

}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

MainMenu::MainMenu(void)
{
	setKeyboardEnabled(true);

}

MainMenu::~MainMenu(void)
{
}