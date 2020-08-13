#pragma once
#include "Sprite.h"

class HotAirBalloon {
private:
	float x, y;
	float width, height;

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;

	Vector2 m_cameraOffset;
	Sprite* m_sprite;
public:
	HotAirBalloon();
	~HotAirBalloon();
	void AssignSprite(int id);
	void Update(float deltaTime);
};