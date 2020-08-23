#pragma once
#include "Sprite.h"
#include "ItemBody.h"

class Enemy : public Sprite
{
protected:
	float x, y;
	float width, height;

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;
	ItemBody* enemyBody;
public:
	Enemy(int id);
	virtual ~Enemy();
	virtual void Update(float deltaTime);
	virtual void createBox2D();
	void setHP(int hp) { this->m_HP = hp; }
	int getHP() { return this->m_HP; }
};

