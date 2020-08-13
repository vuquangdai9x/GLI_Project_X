#pragma once
#include "Sprite.h"

class Player: public Sprite
{
private:
	float x, y;
	float width, height;

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;

	Vector2 m_cameraOffset;

public:
	Player(int id);
	~Player();
	void Update(float deltaTime);
};

