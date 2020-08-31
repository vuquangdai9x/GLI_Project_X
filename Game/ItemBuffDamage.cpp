#include "ItemBuffDamage.h"

ItemBuffDamage::ItemBuffDamage(float damage)
{
	this->damage = damage;
	this->duration = 30;
}

ItemBuffDamage::~ItemBuffDamage()
{
}

void ItemBuffDamage::Use(Player* p)
{
	p->setDamage(this->damage);
	p->setDurationDamage(this->duration);
}
