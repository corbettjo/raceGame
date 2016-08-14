#include "Player.h"

Player::Player(void)
{
	velocity = Point(0, 0);
	position = Point();
	contactedX = false;
	onSideLeft = false;

}

Player::~Player(void)
{
}