#pragma once
#include"Enemy.h"
class VampireBat : public Enemy
{
private:
	Vector2 centerPos;
	Vector2 target;
	float m_range;

	float m_radius;
	b2Vec2 m_velocity;
	float m_maxforce;
	float m_maxSpeed;

	bool m_active;

public:
	VampireBat(int id, Vector2 centerPos, float m_range);
	~VampireBat();
	void Update(float deltaTime);
	void createBox2D();
	bool checkPlayer();
	void move(float moveSpeed);
};

