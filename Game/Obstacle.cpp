#include "Obstacle.h"
#include "Singleton.h"
#include "WorldManager.h"

Obstacle::Obstacle(int id, int type): Sprite(id)
{
	this->m_type = type;
}

Obstacle::~Obstacle()
{
}

void Obstacle::Update(float deltaTime)
{
}

void Obstacle::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(OBSTACLE, x, y, width, height);
}

void Obstacle::createTriangle2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	obstacleBody = Singleton<WorldManager>::GetInstance()->createTriangle(OBSTACLE, x, y, width, height);
}
