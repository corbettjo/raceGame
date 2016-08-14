#include "globals.h"
#include "MainMenu.h"
#include "GameScene.h"
#include "Level.h"
#include "SimpleAudioEngine.h" 
#include <iostream>
#include <fstream>  
#include <string>

using namespace std;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	
	//MUSIC AND EFFECTS
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("inGame.wav", true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("green.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("yellow2.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("red.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pause.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("back.wav");

	//MAP OBJECTS
	levelPartOne = new Level();
	levelPartOne->loadMap("mapPartOne.tmx");
	levelPartOne->retain();
	levelPartOne->getMap()->setScale(2.0f);
	this->addChild(levelPartOne->getMap(), 0);

	levelPartTwo = new Level();
	levelPartTwo->loadMap("mapPartTwo.tmx");
	levelPartTwo->retain();
	levelPartTwo->getMap()->setScale(2.0f);
	this->addChild(levelPartTwo->getMap(), 0);

	levelPartThree = new Level();
	levelPartThree->loadMap("mapPartThree.tmx");
	levelPartThree->retain();
	levelPartThree->getMap()->setScale(2.0f);
	this->addChild(levelPartThree->getMap(), 0);

	levelPartFour = new Level();
	levelPartFour->loadMap("mapPartFour.tmx");
	levelPartFour->retain();
	levelPartFour->getMap()->setScale(2.0f);
	this->addChild(levelPartFour->getMap(), 0);

	tiles = levelPartOne->getCollisionTiles();
	
	//SETS NUMBER OF OBSTACLES BASED ON DIFFICULTY
	if (MainMenu::level == 1)
	{
		red = 25;
		yellow = 50;
		green = 75;
	}
	else if (MainMenu::level == 2)
	{
		red = 50;
		yellow = 100;
		green = 150;
	}
	else if (MainMenu::level == 3)
	{
		red = 100;
		yellow = 200;
		green = 249;
	}


	Point point = Point(36.75, 7); //Start position of car

	//SPRITE FOR CAR
	carSprite = Sprite::create("car.png");
	carSprite->setScale(1.5f);
	carSprite->setAnchorPoint(Point(0, 0));
	carSprite->setPosition(levelPartOne->tileCoordinateToPosition(point));
	this->addChild(carSprite, 2); //Adds the carSprite to the scene

	//PLAYER OBJECT
	player = new Player();
	player->retain();
	player->position = carSprite->getPosition();
	player->player_size.width = carSprite->getBoundingBox().size.width;
	player->player_size.height = carSprite->getBoundingBox().size.height;

	//ASSIGNING BLOCK COLORS INT THE ARRAY BLOCKS[I].MOZART
	for (int i = 0; i <= (red); i++) //Red blocks
	{
		blocks[i].mozart = Sprite::create("Red.png");
		blocks[i].mozart->setTag(0);
	}

	for (int i = (red + 1); i <= (yellow); i++) //Yellow blocks
	{
		blocks[i].mozart = Sprite::create("Yellow.png");
		blocks[i].mozart->setTag(1);
	}

	for (int i = (yellow + 1); i <= (green); i++) //Green blocks
	{
		blocks[i].mozart = Sprite::create("Green.png");
		blocks[i].mozart->setTag(2);
	}

	//GAME CONTROL VARIABLE INITIALIZATIONS
	player->onSideLeft = true;
	carVelocity = 12.0f;
	raceStarted = false; //Game has not started
	paused = false; //Game is not paused
	slowTime = 0; //There is no slow time
	courseTimer = 0;

	//CAMERA OBJECT INITIALIZATIONS
	Point origin = Director::getInstance()->getVisibleOrigin(); //Creates and sets "origin" to be the visible origin of the screen
	Size wsize = Director::getInstance()->getVisibleSize();  //Default screen size (or design resolution size, if you are using design resolution)
	Point *center = new Point(wsize.width / 2 + origin.x, wsize.height / 2 + origin.y);

	//CAMERA TARGET 
	cameraTarget = Sprite::create();
	cameraTarget->setPositionY(wsize.height / 2 + origin.y);
	this->addChild(cameraTarget);
	cameraTarget->retain();

	//PRESS SPACE GRAPHIC
	pressSpace = Sprite::create("pressSpace.png");
	pressSpace->setScale(1.0f);
	pressSpace->setPosition(1190, wsize.height - 200);
	pressSpace->setOpacity(0);
	this->addChild(pressSpace, 4, "pressSpace");
	pressSpace->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), FadeIn::create(0.5f)));

	//PAUSE BUTTON
	pauseButton = MenuItemImage::create("pause.png", "pauseClicked.png", this, menu_selector(GameScene::loadPauseMenu));
	invisibleMenu = Menu::create();
	invisibleMenu->addChild(pauseButton);
	invisibleMenu->setPosition(cameraTarget->getPositionX() + 435, cameraTarget->getPositionY() - 270);
	this->addChild(invisibleMenu, 4);

	//TIMER LABEL
	courseTime = Label::create("TIME: " + to_string(courseTimer), "Impact", 25, CCSizeMake(245, 100), kCCTextAlignmentCenter); //Creates a label for the time
	courseTime->retain(); //Retains the memory space for the label object
	courseTime->setPositionX(cameraTarget->getPositionX() + 400);
	courseTime->setPositionY(cameraTarget->getPositionY() + 230);
	this->addChild(courseTime, 5);

	camera = Follow::create(cameraTarget, Rect::ZERO); //Creates a camera object 

	loadObstacles(); //Calls the method to load the obstacles 

	this->schedule(schedule_selector(GameScene::updateScene)); //Chooses "updateScene" to be the scheduled update method
	this->runAction(camera); //Camera node becomes the target for the action

	if (!Layer::init())
	{
		return false;
	}

	return true;
}

void GameScene::loadObstacles()
{
	srand(time(NULL)); //Initializes randomizer
	for (int i = 0; i < (green + 1); i++) //Loop to randomly set the position of the sprites
	{
		int x = (rand() % (47 - 28)) + 28; //Creates a random point horizontally between 27 and 47
		int y = (rand() % (988 - 50)) + 50; //Creates a random point vertically between 40 and 987

		blocks[i].mozart->setPosition(levelPartOne->tileCoordinateToPosition(Point(x, (y)))); //Set the sprite to that position
		blocks[i].mozart->setAnchorPoint(Point::ZERO); //Sets the anchor point 
		blocks[i].mozart->setScale(1.4f); //Sets the scale 

		this->addChild(blocks[i].mozart, 1); //Adds the sprite to the scene
	}
}

void GameScene::updateScene(float delta) //Gets called 60 times every second; Essentially main method
{

	courseTime->setPosition(cameraTarget->getPositionX() + 425, cameraTarget->getPositionY() + 255); //Updates the position of timer label
	invisibleMenu->setPosition(cameraTarget->getPositionX() + 435, cameraTarget->getPositionY() - 280); //Updates the position of the pause button

	if (!paused) //If the game is not paused
	{
		if ((player->position.y + player->player_size.height) <= 31510) //If the player has not crossed the finished line
		{
			this->moveObstacles(); //Calls the method to move the red blocks
			this->updatePlayer(delta); //Calls the method to update the player 
			if (raceStarted) //If the current round is still running
			{
				courseTimer += 1; //Update the counter for the round
				courseTime->setString("TIME: " + to_string(player->onSideLeft)); //Update the label for the time 
			}

			if ((slowTime == 100)) //If the car hit a yellow space, and served its time being slow
			{
				slowTime = 0; //Reset the slowtime
				carVelocity = 12.0f; //Set the speed of the car to be normal 
				player->velocity.y = carVelocity; //Update the player object's speed accordingly
			}

			if (slowTime > 100) //If the car has not yet served the alloted time
			{
				slowTime -= 1; //Continue decrementing the counter
				carVelocity += 0.05f; //Slowly start increasing the speed 
				player->velocity.y = carVelocity; //Update the player object's speed accordingly
			}
		}
		else //If the player has completed the course
		{
			raceStarted = false; //Stop the game 
			loadFinishedMenu(saveScore((courseTimer / 60))); //Load the finished menu
			this->unscheduleAllSelectors(); //Stop the scheduled updater for this method
		}
	}
}

void GameScene::moveObstacles()
{
	//RED BLOCKS ACT AS A MARQUEE. HALF MOVE LEFT, HALF MOVE RIGHT
	for (int i = 0; i <= (red); i++) 
	{
		if (i <= (red / 2)) //First half of red blocks
		{
			if (blocks[i].mozart->getPositionX() >= 1472) //Position of right wall
			{
				blocks[i].mozart->setPositionX(898); //Restart next to the left wall
			}
			blocks[i].mozart->setPositionX(blocks[i].mozart->getPositionX() + 1); //Move one to the right
		}

		else //Second half of red blocks
		{
			if (blocks[i].mozart->getPositionX() <= 898) //Position of left wall
			{
				blocks[i].mozart->setPositionX(1472); //Restart next to right wall
			}
			blocks[i].mozart->setPositionX(blocks[i].mozart->getPositionX() - 1); //Move one to the left
		}
	}
}

void GameScene::updatePlayer(float delta)
{
	Rect player_rect = carSprite->getBoundingBox();

	//DETERMINES WHICH HALF OF TRACK PLAYER IS ON
	if (player->position.x < 1200)
	{
		player->onSideLeft = true; //Left side of course
	}
	else if (player->position.x > 1200)
	{
		player->onSideLeft = false; //Right side of course
	}
		
	//COLLISION CHECK FOR WALL TILES
	for (Rect tile : tiles)
	{
		if (player_rect.intersectsRect(tile)) //If the player intersects...
		{
			if (!player->contactedX) //And has not yet contacted a block...
			{
				player->velocity.x = 0; //Set the veloctiy to 0;
				player->contactedX = true; //Record that it has touched a block
				break;
			}
		}
		else
		{
			player->contactedX = false; //If it does not intersect, it is not contacting a block
		}
	}
	
	//SPACE KEY EVENT
	if (std::find(heldKeys.begin(), heldKeys.end(), SPACEBAR) != heldKeys.end()) //SPACEBAR
	{
		this->removeChildByName("pressSpace");
		if (player->velocity.y <= 0) //If the car is not moving...
		{
			player->velocity.y = carVelocity; //Set its velocity to be moving
			raceStarted = true;
		}
	}

	//RIGHT ARROW KEY EVENT
	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) //RIGHT ARROW
	{
		if ((player->contactedX) && (player->onSideLeft == false)) //If the player is contacting a block, and it is the RIGHT side of the map...
		{
		  //Do nothing
		}
		else //If the player is not intersecting a block on the right side of the map
		{
			player->velocity.x = 5.0f; //Set the player's velocity to be five
		}
	}

	//LEFT ARROW KEY EVENT
	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) //LEFT ARROW
	{
		if ((player->contactedX) && (player->onSideLeft == true)) //If the player is contacting a block, and it is the LEFT side of the map...
		{
			//Do nothing
		}
		else //If the player is not intersecting a block on the left side of the map
		{
			player->velocity.x = -5.0f; //Set the player's velocity to be negative five
		}
	}

	//COLLISION CHECK FOR RANDOMLY GENERATED SPRITES
	for (int i = 0; i <= (green); i++) //"green" is also the size of the array
	{
		if (blocks[i].mozart->getBoundingBox().intersectsRect(player_rect)) //If the player intersects with current obstacle block
		{

			if (blocks[i].mozart->getTag() == 0)//Red 
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("red.wav"); //Play sound
				player->position.y = blocks[i].mozart->getPositionY() - player->player_size.height - 1; //Move the player back on so they are not colliding anymore
				player->velocity.y = 0; //Stop moving the plater forward
				break;
			}

			if (blocks[i].mozart->getTag() == 1)  //Yellow
			{

				if (slowTime < 100) //If the car has not collided before
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("yellow2.wav"); 
				}

				carVelocity = 5.0f; //Slow the temp speed variable
				player->velocity.y = carVelocity; //Update the player's speed
				if (slowTime < 100) //If the car had been at zero (no slowTime)...
				{
					slowTime = 100; //Set it to 100
				}
				slowTime += 5; //Increment by five (increases quickly)
				break;
			}

			if (blocks[i].mozart->getTag() == 2) //Green
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("green.wav");
				slowTime = 0; //Instant snap our of any slowTime owed
				carVelocity = 20.0f; //Double the temp speed
				player->velocity.y = carVelocity; //Update the temp speed
				break;
			}
		}
	}

	player->position.y = player->position.y + player->velocity.y; //Update player's y position
	cameraTarget->setPositionY(carSprite->getPosition().y + 200); //Update camera target's y position
	player->position.x = player->position.x + player->velocity.x;
	cameraTarget->setPositionX(carSprite->getPosition().x + 27);

	carSprite->setPositionY(player->position.y); //Update the carSprite's y position
	carSprite->setPositionX(player->position.x);
	

	//IF AN ARROW IS RELEASED, STOP MOVING THE PLAYER
	if ((std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) == heldKeys.end()) || (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) == heldKeys.end()))
	{
		player->velocity.x = 0;
	}

}

int GameScene::saveScore(int y)
{
	int start = 0;
	//DETERMINES WHOCH SET OF SCORES TO ACCESS BASED ON LEVEL
	if (MainMenu::level == 1)
	{
		start = 0; //Easy scores (0-2)
	}
	if (MainMenu::level == 2)
	{
		start = 3; //Medium scores (2-5)
	}
	if (MainMenu::level == 3)
	{
		start = 6; //Hard scores (6-8)
	}
	int theCarsPlace = 0; //Place of potential new high score
	string line; 
	ifstream inScore("scores.txt");
	int temps[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //Temporary score array to hold values between reading and writing of file
	int x = 0;

	if (inScore.is_open()) //Boolean check to see if the file stream is open 
	{
		while ((getline(inScore, line))) //While there are still ines to be read
		{
			temps[x] = stoi(line);
			x++; //Incremeted for different positions in array
		}

		inScore.close();

		ofstream outScore("scores.txt");

		if (outScore.is_open())
		{
			for (int p = start; p < (start + 3); p++) //From g, to twoo spots after g
			{
				if (y < temps[p]) //If the player's score for the race is les sthan a high score
				{
					temps[p] = y; //Replace that old high score with a new one
					theCarsPlace = p + 1; //Return the location of that score
					break;
				}
			}

			for (int r = 0; r < 9; r++)
			{
				outScore << temps[r] << endl; //Replace scores
			}
		}

		outScore.close();
		return theCarsPlace;
	}
}

std::string GameScene::scoreReporter(int place)
{
	if ((place == 1) || (place == 4) || (place == 7)) //First place
	{
		return("NEW HIGH SCORE! TOP TIME!");
	}
	else if ((place == 2) || (place == 5) || (place == 8)) //Second Place
	{
		return("NEW HIGH SCORE! SECOND BEST TIME!");
	}
	else if ((place == 3) || (place == 6) || (place == 9)) //Third Place
	{
		return("NEW HIGH SCORE! THIRD BEST TIME!");
	}
	else if (place == 0) //No High Score
	{
		return("NO NEW HIGH SCORE! WELL DONE!");
	}
}

void GameScene::loadPauseMenu(Ref *psender)
{

	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pause.wav");

	//PAUSES THE GAME AND DISABLES BUTTON
	pauseButton->setEnabled(false);
	paused = true;

	//SHADER SPRITE
	shader = Sprite::create("shader.png");
	shader->setScale(1.0f);
	shader->setPosition(cameraTarget->getPositionX(), cameraTarget->getPositionY());
	this->addChild(shader, 4, "shader");

	//MENU BASE SPRITE
	menuBaseSprite = Sprite::create("PauseMenuBase.png");
	menuBaseSprite->setScale(1.0f);
	menuBaseSprite->setPosition(cameraTarget->getPositionX(), cameraTarget->getPositionY());
	this->addChild(menuBaseSprite, 5, "pauseMenuBase");


	//Creates the buttons for the menu
	resumeButton = MenuItemImage::create("resumeButton.png", "resumeButtonClicked.png", this, menu_selector(GameScene::resumeGame));
	resumeButton->setPosition(menuBaseSprite->getPositionX() - 480, cameraTarget->getPositionY() - 250);
	resumeButton->setScale(1.0f);
	restartRaceButton = MenuItemImage::create("restartRaceButton.png", "restartRaceButtonClicked.png", this, menu_selector(GameScene::restartRace));
	restartRaceButton->setPosition(cameraTarget->getPositionX() - 480, cameraTarget->getPositionY() - 300);
	restartRaceButton->setScale(1.0f);
	mainMenuButton = MenuItemImage::create("mainMenuButton.png", "mainMenuButtonClicked.png", this, menu_selector(GameScene::mainMenu));
	mainMenuButton->setPosition(cameraTarget->getPositionX() - 480, cameraTarget->getPositionY() - 350);
	mainMenuButton->setScale(1.0f);
	quitButton = MenuItemImage::create("quitButton.png", "quitButtonClicked.png", this, menu_selector(GameScene::menuCloseCallback));
	quitButton->setPosition(menuBaseSprite->getPositionX() - 480, cameraTarget->getPositionY() - 400);
	quitButton->setScale(1.0f);

	// Creating a vector to hold the menu items
	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(resumeButton);
	vec.pushBack(restartRaceButton);
	vec.pushBack(mainMenuButton);
	vec.pushBack(quitButton);

	//Puts the buttons onto the scene
	cocos2d::Menu* menu = Menu::createWithArray(vec);
	menu->setScale(1.0f);
	this->addChild(menu, 6, "menu");
}

void GameScene::loadFinishedMenu(int carPlace)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("finished.wav", true);

	//CLOSES PAUSE BUTTON OBJECTS
	pauseButton->setEnabled(false);
	pauseButton->setVisible(false);

	//SHADER SPRITE
	shader = Sprite::create("shader.png");
	shader->setScale(1.0f);
	shader->setPosition(cameraTarget->getPositionX(), cameraTarget->getPositionY());
	this->addChild(shader, 4, "shader");

	//CREATES AND LOADS BASE FOR MENU
	menuBaseSprite = Sprite::create("finishedMenuBase.png");
	menuBaseSprite->setScale(1.0f);
	menuBaseSprite->setPosition(cameraTarget->getPositionX(), cameraTarget->getPositionY());
	menuBaseSprite->setTag(1);
	this->addChild(menuBaseSprite, 5);

	//REPORTS TIME
	courseTime->setScale(1.0f);
	courseTime->setString("YOUR TIME: " + to_string(courseTimer / 60));
	courseTime->setPosition(cameraTarget->getPositionX(), cameraTarget->getPositionY() - 15); //Updates the x position of timer label
	courseTime->setZOrder(10);
	//CREATES AND LOADS LABEL TO REPORT SCORE
	scoreReport = Label::create(scoreReporter(carPlace), "Impact", 40, CCSizeMake(640, 100), kCCTextAlignmentCenter); //Calls score reporter method
	scoreReport->setScale(1.0f);
	scoreReport->setPositionX(cameraTarget->getPositionX());
	scoreReport->setPositionY(cameraTarget->getPositionY() + 60);
	this->addChild(scoreReport, 6);

	//CREATES MENU BUTTONS
	restartRaceButton = MenuItemImage::create("restartRaceButton.png", "restartRaceButtonClicked.png", this, menu_selector(GameScene::restartRace));
	restartRaceButton->setPosition(cameraTarget->getPositionX() - 480, cameraTarget->getPositionY() - 380);
	restartRaceButton->setScale(1.0f);
	mainMenuButton = MenuItemImage::create("mainMenuButton.png", "mainMenuButtonClicked.png", this, menu_selector(GameScene::mainMenu));
	mainMenuButton->setPosition(cameraTarget->getPositionX() - 480, cameraTarget->getPositionY() - 430);
	mainMenuButton->setScale(1.0f);
	quitButton = MenuItemImage::create("quitButton.png", "quitButtonClicked.png", this, menu_selector(GameScene::menuCloseCallback));
	quitButton->setPosition(menuBaseSprite->getPositionX() - 480, cameraTarget->getPositionY() - 480);
	quitButton->setScale(1.0f);

	//CREATES VECOTR TO HOLD BUTTONS
	cocos2d::Vector<cocos2d::MenuItem*> vec;
	vec.pushBack(restartRaceButton);
	vec.pushBack(mainMenuButton);
	vec.pushBack(quitButton);

	//CREATES AND LOADS MENU ITEM FOR BUTTONS
	cocos2d::Menu* finishedMenu = Menu::createWithArray(vec);
	finishedMenu->setScale(1.0f);
	this->addChild(finishedMenu, 6);
}

void GameScene::resumeGame(Ref *psender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("back.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

	//RE-ENABLES PAUSE OBJECTS
	paused = false;
	pauseButton->setEnabled(true);

	//REMOVES PAUSE MENU OBJECTS
	this->removeChildByName("pauseMenuBase");
	this->removeChildByName("menu");
	this->removeChildByName("shader");
}

void GameScene::restartRace(Ref *psender)
{
	//STOPS GAME AND RESTARTS THE SCENE
	this->unscheduleAllSelectors();
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitRows::create(1.0f, scene));
}

void GameScene::mainMenu(Ref *psender)
{
	//STOPS THE GAME AND REPLACES GAMESCENE WITH MANI MENU SCENE
	this->unscheduleAllSelectors();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainMenu::createScene()));
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()){
		heldKeys.push_back(keyCode);
	}

}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

GameScene::GameScene(void)
{
	setKeyboardEnabled(true);
}

GameScene::~GameScene(void)
{
}