#pragma once
#include "Sprite.h"
#include "ItemBody.h"
class Player: public Sprite
{
private:
	float x, y;
	float width, height;

	float pre_KeyX = 0.0f;
	float pre_KeyY = 0.0f;

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;
	ItemBody* playerBody;
	Vector2 m_cameraOffset;


public:
	Player(int id);
	~Player();
	void Update(float deltaTime);
	void createBox2D();
};

