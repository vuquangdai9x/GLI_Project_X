#pragma once
#include "Sprite.h"
#include "ItemBody.h"
#include "Player.h"
class Bullet : public Sprite
{
protected:
	float m_mass = 0;
	float m_damage = 0;
	float m_initSpeed = 0;
	ItemBody* bulletBody = NULL;
	Player* m_player = NULL;
public:
	Bullet(int id, float mass, float damage, float initSpeed);
	Bullet(int id, Bullet& bullet);
	virtual ~Bullet();
	virtual void InitPhysics() = 0;
	virtual void Fire(Player* player, Vector2 startPos, Vector2 direction) = 0;
	virtual void SetActiveBullet(bool value) = 0;
	virtual void Update(float deltaTime);
};

