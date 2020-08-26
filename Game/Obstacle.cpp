#include "Obstacle.h"
#include "Singleton.h"
#include "WorldManager.h"
#include"UserData.h"
Obstacle::Obstacle(int id, int type): Sprite(id)
{

	this->m_type = type;
}

Obstacle::~Obstacle()
{
}

void Obstacle::Update(float deltaTime)
{
	Sprite::Update(deltaTime);
}

void Obstacle::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(OBSTACLE, x, y, width, height);
	UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
	user->m_damage = 0;
}

void Obstacle::createTriangle2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	obstacleBody = Singleton<WorldManager>::GetInstance()->createTriangle(OBSTACLE, x, y, width, height);
	UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
	user->m_damage = 0;
}
