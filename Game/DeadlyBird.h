#pragma once
#include "Enemy.h"
class DeadlyBird : public Enemy
{
public: 
	DeadlyBird(int id);
	~DeadlyBird();
	void Update(float deltaTime);
	void Seek(Vector2 target,Vector2 v);
	void createBox2D();
private:
	Vector2 m_target = Vector2(3, 0), m_up = Vector2(1, 0);
	int active = 0;
	float m_activeTime = 0;
	b2Vec2 m_attackVector;
};

