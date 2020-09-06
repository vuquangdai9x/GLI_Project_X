#include "Obstacle.h"
#include "Singleton.h"
#include "WorldManager.h"
#include"UserData.h"
Obstacle::Obstacle(int id, int type): Sprite(id)
{
	this->m_type = type;
	obstacleBody = NULL;
	m_damage = 0;
}

Sprite* Obstacle::CreateClone(int iNewId)
{
	Obstacle* clone = new Obstacle(iNewId, this->m_type);
	clone->Init(*this);
	clone->SetDamage(this->m_damage);
	clone->CreatePhysicsBody(this->m_obstacleType, this->m_mass);
	return clone;
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
	obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(OBSTACLE, x, y, width, height, 5, WorldManager::BodyType::Static);
	UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
	user->m_damage = 0;
}

void Obstacle::createTriangle2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	obstacleBody = Singleton<WorldManager>::GetInstance()->createTriangle(OBSTACLE, x, y, width, height, 5, WorldManager::BodyType::Static);
	UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
	user->m_damage = m_damage;
}

void Obstacle::SetDamage(float damage) {
	m_damage = damage;
	if (obstacleBody != NULL) {
		UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
		user->m_damage = m_damage;
	}
}

void Obstacle::CreatePhysicsBody(ObstacleType obsType, float mass) {
	m_obstacleType = obsType;
	m_mass = mass;
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	switch (m_obstacleType)
	{
	case ObstacleType::Cliff:
		obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(
			OBSTACLE, 
			x, y, 
			width * 0.9, 
			height * 0.95, 
			mass,
			WorldManager::BodyType::Static
		);
		break;
	case ObstacleType::Island:
		obstacleBody = Singleton<WorldManager>::GetInstance()->createTriangle(
			OBSTACLE,
			x, y, 
			width, 
			height * 0.95,
			mass,
			WorldManager::BodyType::Static
		);
		break;
	case ObstacleType::Piece:
	default:
		obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(
			OBSTACLE,
			x, y,
			width * 0.8,
			height * 0.8,
			mass,
			WorldManager::BodyType::Dynamic
		);
		break;
	}
	UserData* user = (UserData*)this->obstacleBody->body->GetUserData();
	user->m_damage = m_damage;
}
