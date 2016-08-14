#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Player : public Object
{
public:
	Size player_size; //Size of the player (for scaling)
	Point position; //Position of the player
	Point velocity; //Velocity of the player
	bool contactedX; //Check for if the player has collided with a wall 
	bool onSideLeft; //Check for if the player is on the left or right side of the map

	Player(void);
	virtual ~Player(void);
};
