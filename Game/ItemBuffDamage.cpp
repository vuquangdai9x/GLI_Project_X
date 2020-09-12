#include "ItemBuffDamage.h"

ItemBuffDamage::ItemBuffDamage(float damage)
{
	this->damage = damage;
	this->duration = 15;
}

ItemBuffDamage::~ItemBuffDamage()
{
}

void ItemBuffDamage::Use(Player* p)
{
	p->setDamage(this->damage);
	p->setDurationDamage(this->duration);
}
