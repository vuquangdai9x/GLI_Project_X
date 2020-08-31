#pragma once
#include"Item.h"
class ItemAmoGun : public Item
{
private:
	int gunBullet;
	int canonBullet;
public:
	ItemAmoGun(int gunBullet, int canonBullet);
	~ItemAmoGun();
	void Use(Player* p);
};

