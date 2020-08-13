#pragma once
#include "Sprite.h"

class Player: public Sprite
{
private:
	float x, y;
	float width, height;

	int hp, maxhp;
	int dmg;
	float moveSpeed;

public:
	Player();
	~Player();
	void update(float deltaTime);
	void render();
	void KeyPress();
};

