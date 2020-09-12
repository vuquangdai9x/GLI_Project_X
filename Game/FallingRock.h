#pragma once
#include"Bullet.h"
#include <stdlib.h>
class FallingRock : public Bullet
{
protected:
	float m_mass;
	float m_gravityScale;
	float m_damage;
	float m_initSpeed;
	float m_existTime;

	float m_timeCounter;
public:
	FallingRock(int id, float mass, float gravityScale, float damage, float initSpeed, float existTime);
	FallingRock(int id, FallingRock& templateBullet);
	virtual ~FallingRock();
	void CreatePhysicsBody();
	void Fire(int index, Sprite* shooter, Vector2 startPosition, Vector2 direction);
	void Update(float deltaTime);
	int time = 0;
};

