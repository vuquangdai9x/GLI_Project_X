#pragma once
#include "Sprite.h"
#include "ItemBody.h"
class Obstacle: public Sprite
{
private:
	float x, y;
	float width, height;

	float m_moveSpeed;
	int m_type;

	ItemBody* obstacleBody;
public:

	Obstacle(int id, int type);
	~Obstacle();
	void Update(float deltaTime);
	void createBox2D();
	void createTriangle2D();
};

