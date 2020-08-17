#pragma once
#include "Sprite.h"
#include "ItemBody.h"
class Bullet : public Sprite
{
protected:
	float x, y;
	float width, height;

	int m_damage;
	float m_moveSpeed;
	ItemBody* bulletBody;
public:
	Bullet();
	Bullet(b2Vec2 startPosition);
	~Bullet();
	virtual void createBox2D();
	virtual void Fire(b2Vec2 direction);
	virtual void Update(float deltaTime);
};

