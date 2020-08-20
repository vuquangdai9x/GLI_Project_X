#pragma once
#include"Enemy.h"
#include "Weapon.h"
class RockyGolem : public  Enemy
{
private:
	long time = 0;
	Weapon *rockyGolemGun;
public:
	RockyGolem(int id);
	~RockyGolem();
	void Update(float deltaTime);
	void createBox2D();
	void createWeapon();
};

