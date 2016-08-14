#pragma once

#include "cocos2d.h"
#include "Player.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class MainMenu : public cocos2d::Layer
{
public:
	
	static int level; //(1:Easy/2:Medium/3:Hard)
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	void updateScene(float interval);
	void menuCloseCallback(cocos2d::Ref* psender);
	
	//LOAD MENUS 
	void loadTopSelectMenu(); //Loads the main menu 
	void loadLevelSelectMenu(Ref *psender); //Loads the menu to select levels of different difficulties
	void loadEasyLevel(Ref *psender); //Number of Blocks: 75
	void loadMediumLevel(Ref *psender); //Number of Blocks: 150
	void loadHardLevel(Ref *psender); //Number of Blocks: 250
	void loadAboutMenu(Ref *psender); //Loads about menu
	void loadControlsMenu(Ref *psender); //Loads controls menu

	//SPRITES FOR MENU BACKGROUNDS/BASES
	//MULTIPLE SPRITES ALLOW FOR MANUAL FADE IN/OUT TRANSITIONS
	Sprite *startScreen_sprite;
	Sprite *topMenuBase_sprite;
	Sprite *levelSelectMenuBase_sprite;
	Sprite *aboutMenuBase_sprite;
	Sprite *controlsMenuBase_sprite;
	Sprite *menuBase_sprite;

	//MENU ITEMS FOR MAIN MENU
	MenuItem *levelSelectButton;
	MenuItem *controlsButton;
	MenuItem *aboutButton;
	MenuItem *quitBigButton;

	//MENU ITEMS FOR SUB-MENUS
	MenuItem *levelEasyButton;
	MenuItem *levelMediumButton;
	MenuItem *levelHardButton;
	MenuItem *backButton;
	
	//MEDTHODS TO RETURN TO MAIN MENU 
	void levelSelectBack(Ref *psender);
	void aboutBack(Ref *psender);
	void controlsBack(Ref *psender);

	//KEY PRESS HANDLERS
	vector<EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
	CREATE_FUNC(MainMenu); //Makes create functions

	MainMenu(void);
	virtual ~MainMenu(void);
};