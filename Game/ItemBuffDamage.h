#pragma once
#include"Item.h"
class ItemBuffDamage : public Item
{
private:
	float damage;
	float duration;
public:
	ItemBuffDamage(float damage);
	~ItemBuffDamage();
	void Use(Player* p);
};


