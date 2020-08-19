#pragma once
#include"Bullet.h"
#include <stdlib.h>
class GunBullet : public Bullet
{
private:
	float m_existTime;
public:
	GunBullet(int id, float mass, float damage, float initSpeed);
	GunBullet(int id, GunBullet& templateBullet);
	~GunBullet();
	void InitPhysics();
	void Fire(Player* player, Vector2 startPosition, Vector2 direction);
	void SetActiveBullet(bool value);

	void Update(float deltaTime);
};

