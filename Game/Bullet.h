#pragma once
#include "Sprite.h"
#include "ItemBody.h"
#include "Player.h"
class Bullet : public Sprite
{
protected:
	float m_mass;
	float m_damage;
	float m_initSpeed;
	ItemBody* bulletBody;
	Player* m_player;
public:
	Bullet(int id, float mass, float damage, float initSpeed);
	Bullet(int id, Bullet& bullet);
	virtual ~Bullet();
	virtual void InitPhysics() = 0;
	virtual void Fire(Player* player, Vector2 startPos, Vector2 direction) = 0;
	virtual void SetActiveBullet(bool value) = 0;
	virtual void Update(float deltaTime);
};

