#pragma once
#include"Bullet.h"
#include <stdlib.h>
class GunBullet : public Bullet
{
protected:
	float m_mass;
	float m_gravityScale;
	float m_damage;
	float m_initSpeed;
	float m_existTime;

	float m_timeCounter;
public:
	GunBullet(int id, float mass, float gravityScale, float damage, float initSpeed, float existTime);
	GunBullet(int id, GunBullet& templateBullet);
	virtual ~GunBullet();
	void CreatePhysicsBody();
	void Fire(Sprite* shooter, Vector2 startPosition, Vector2 direction);
	void Update(float deltaTime);
	int time = 0;
};

