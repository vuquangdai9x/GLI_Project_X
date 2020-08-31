#include "ItemBuffSpeed.h"

ItemBuffSpeed::ItemBuffSpeed(float moveSpeed, float flySpeed)
{
	this->moveSpeed = moveSpeed;
	this->flySpeed = flySpeed;
	this->duration = 30;
}

ItemBuffSpeed::~ItemBuffSpeed()
{
}

void ItemBuffSpeed::Use(Player* p)
{
	p->setMoveSpeed(this->moveSpeed);
	p->setFlySpeed(this->flySpeed);
	p->setDurationSpeed(this->duration);
}
