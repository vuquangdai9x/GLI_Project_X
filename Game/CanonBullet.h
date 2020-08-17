#pragma once
#include"Bullet.h"
#include <stdlib.h>
class CanonBullet : public Bullet
{
public:
	CanonBullet(b2Vec2 startPosition, float angle);
	~CanonBullet();
	void createBox2D();
	void Fire(b2Vec2 direction);
	void Update(float deltaTime);
	int time = 0;
};

