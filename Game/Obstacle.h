#pragma once
#include "Sprite.h"
#include "ItemBody.h"
class Obstacle: public Sprite
{
public:
	enum ObstacleType { Piece, Island, Cliff };
private:
	float x, y;
	float width, height;
	int m_type;

	ObstacleType m_obstacleType;
	float m_damage;
	float m_mass;

	ItemBody* obstacleBody;
public:
	Obstacle(int id, int type);
	virtual Sprite* CreateClone(int iNewId);
	~Obstacle();
	void Update(float deltaTime);
	void createBox2D();
	void createTriangle2D();
	void CreatePhysicsBody(ObstacleType obsType, float mass = 5);
	void SetDamage(float damage);
};

