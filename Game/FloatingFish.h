#pragma once
#include"Enemy.h"

class FloatingFish : public Enemy
{
private:
	int numOfTarget;
	b2Vec2* target;
	int currentTarget;
	b2Vec2 velocity;
	float m_maxforce;

	bool direction;
public:
	FloatingFish(int id, int numOfTarget, b2Vec2 target[]);
	~FloatingFish();
	void Update(float deltaTime);
	void createBox2D();
};

