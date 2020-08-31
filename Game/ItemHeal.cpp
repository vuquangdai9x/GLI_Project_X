#include "ItemHeal.h"

ItemHeal::ItemHeal(int amount)
{
	this->amount = amount;
}

ItemHeal::~ItemHeal()
{
}

void ItemHeal::Use(Player* p)
{
	int hp = p->getHP() + this->amount;
	if (hp > p->getMaxHP()) {
		p->setHP(p->getMaxHP());
	}
	else {
		p->setHP(hp);
	}
}
