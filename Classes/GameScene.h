#pragma once
#include "cocos2d.h"
#include "Level.h"
#include "Player.h"

#include <string>
#include <vector>

using namespace std;
USING_NS_CC;

struct obstacles
{
	Sprite *mozart; //Named after who taught me about data structures

};

class GameScene : public cocos2d::Layer
{
public:

	//GAME METHODS
	virtual bool init();
	void loadObstacles(); //Places obstacles on the scene in a random arrangement
	void updateScene(float interval); //Called 60 times a second
	void updatePlayer(float interval);
	void moveObstacles(); //Moves the red blocks
	void loadFinishedMenu(int place);
	int saveScore(int y); //Parses a text file and comapares player's score to the high scores
	std::string scoreReporter(int place);

	//MENU METHODS
	void loadPauseMenu(Ref *psender);
	void resumeGame(Ref *psender);
	void restartRace(Ref *psender);
	void mainMenu(Ref *psender);
	void quit(Ref *psender);
	void menuCloseCallback(cocos2d::Ref* psender); //Quits the program

	//LEVEL OBJECTS (Four required due to rendering bug)
	Level *levelPartOne;
	Level *levelPartTwo;
	Level *levelPartThree;
	Level *levelPartFour;
	vector<Rect> tiles; //Rectangle vector for wall tiles

	//IN-GAME OBJECTS
	Label* courseTime; //Displays player's time 
	int courseTimer; //Keeps track of player's time
	bool raceStarted; //Check if the race has started
	bool paused; //Check if the game is paused
	
	//PLAYER OBJECTS
	Player *player;
	Sprite *carSprite;
	Sprite *cameraTarget;
	Follow *camera; //Action that follows a sprite node
	Point center;
	float carVelocity; //Y-axis speed of the car
	int slowTime; //Amount of time the car will travel at a slower speed. Accumulates based on how long car is on yellow space
	
	//MENU GRAPHICS
	Sprite *pressSpace; //"Press Space to Start!" sprite
	Sprite *shader; //Adds darker background for menus
	Sprite *menuBaseSprite; 
	Menu *invisibleMenu; //base for the pause button

	//MENU ITEMS
	MenuItem *pauseButton;
	MenuItem *resumeButton;
	MenuItem *restartRaceButton;
	MenuItem *mainMenuButton;
	MenuItem *quitButton;
	Label* scoreReport;

	//OBSTACLE OBJECTS
	obstacles blocks[250];
	int green; //Varies based on difficulty (25, 50, 50)
	int yellow; //Varies based on difficulty (25, 50, 100)
	int red; //Varies based on difficulty (25, 50, 100)

	vector<EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	static cocos2d::Scene* createScene();

	CREATE_FUNC(GameScene);

	GameScene(void);
	virtual ~GameScene(void);
};