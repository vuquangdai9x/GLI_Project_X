#pragma once
#include "Sprite.h"
class Obstacle: public Sprite
{
private:
	float x, y;
	float width, height;

	float m_moveSpeed;
	int m_type;

public:
	Obstacle(int type);
	~Obstacle();
	void Update(float deltaTime);
};

