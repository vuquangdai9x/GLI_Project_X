#pragma once
#include "Sprite.h"
#include "ItemBody.h"
#include"UserData.h"
class Enemy : public Sprite
{
protected:
	float x, y;
	float width, height;
	int basic_Dmg;

	DWORD m_TakeDameTime = 0;
	int m_animationTime = 200;
	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;
	ItemBody* enemyBody;
public:
	Enemy(int id);
	virtual ~Enemy();
	virtual void Update(float deltaTime);
	virtual void createBox2D();
	void takeDamage();
	void setHP(int hp) { this->m_HP = hp; }
	int getHP() { return this->m_HP; }
};

