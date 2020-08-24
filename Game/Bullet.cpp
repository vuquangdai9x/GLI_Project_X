#include "Bullet.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"

Bullet::Bullet(int id): Sprite(id), m_player(NULL), m_bulletBody(NULL)
{}

Bullet::~Bullet()
{}

void Bullet::SetActiveBullet(bool value)
{
	m_bulletBody->setActive(value);
	SetActiveSprite(value);
}

void Bullet::Update(float deltaTime)
{
	if (m_bulletBody != NULL) {
		Vector3 pos;
		pos.x = m_bulletBody->body->GetPosition().x;
		pos.y = m_bulletBody->body->GetPosition().y;
		pos.z = GetPosition().z;
		this->SetPosition(pos);
		this->SetRotation(m_bulletBody->body->GetAngle());
	}
}
