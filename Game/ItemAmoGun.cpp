#include "ItemAmoGun.h"
#include "Singleton.h"
#include "SceneManager2D.h"
ItemAmoGun::ItemAmoGun(int gunBullet, int canonBullet)
{
	this->gunBullet = gunBullet;
	this->canonBullet = canonBullet;
}

ItemAmoGun::~ItemAmoGun()
{
}

void ItemAmoGun::Use(Player* p)
{
	Singleton<SceneManager2D>::GetInstance()->getCombatController()->AddBullet(0, this->gunBullet);
	Singleton<SceneManager2D>::GetInstance()->getCombatController()->AddBullet(1, this->canonBullet);
}
