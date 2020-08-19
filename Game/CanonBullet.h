#pragma once
#include"Bullet.h"
#include <stdlib.h>
class CanonBullet : public Bullet
{
public:
	CanonBullet(int id, float mass, float damage, float initSpeed);
	CanonBullet(int id, CanonBullet& templateBullet);
	~CanonBullet();
	void InitPhysics();
	void Fire(Player* player, Vector2 startPosition, Vector2 direction);
	void SetActiveBullet(bool value);
};

