#pragma once
#include "Player.h"
class Item
{
public:
	Item();
	~Item();
	virtual void Use(Player *p);
};

