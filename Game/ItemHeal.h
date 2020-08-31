#pragma once
#include"Item.h"
class ItemHeal : public Item
{
private:
	int amount;
public:
	ItemHeal(int amount);
	~ItemHeal();
	void Use(Player* p);
};

