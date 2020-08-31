#pragma once
#include"Item.h"
class ItemBuffSpeed: public Item
{
private:
	float moveSpeed;
	float flySpeed;
	float duration;
public:
	ItemBuffSpeed(float moveSpeed, float flySpeed);
	~ItemBuffSpeed();
	void Use(Player* p);
};

