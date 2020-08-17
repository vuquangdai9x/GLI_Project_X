#pragma once
#include"Bullet.h"
#include <stdlib.h>
class GunBullet : public Bullet
{
public:
	GunBullet(b2Vec2 startPosition, float angle);
	~GunBullet();
	void createBox2D();
	void Fire(b2Vec2 direction);
	void Update(float deltaTime);
};

