#pragma once
#include"Enemy.h"
class VampireBat : public Enemy
{
private:
	Vector2 startPos, endPos;

	float m_radius;
	b2Vec2 m_velocity;
	float m_maxforce;
	float m_maxSpeed;
	bool direction;
	bool m_active;
public:
	VampireBat(int id, Vector2 startPos, Vector2 endPos);
	~VampireBat();
	void Update(float deltaTime);
	void createBox2D();
	bool checkPlayer();
};

