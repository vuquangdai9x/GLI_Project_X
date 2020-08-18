#include "CanonBullet.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"

CanonBullet::CanonBullet(b2Vec2 startPosition, float angle)
{
	Vector3 bulletPos;
	bulletPos.x = startPosition.x;
	bulletPos.y = startPosition.y;
	bulletPos.z = 0;
	float rotation = angle * 2 * M_PI / 360;
	this->Init(bulletPos, rotation, Vector2(0.5, 0.5), 0xffffff, 1, 0, 10);
	this->createBox2D();
	m_moveSpeed = 15.0;
}

CanonBullet::~CanonBullet()
{
}

void CanonBullet::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	this->bulletBody = Singleton<WorldManager>::GetInstance()->createRectagle(CANONBULLET, x, y, width, height, 8);
}

void CanonBullet::Fire(b2Vec2 direction)
{
	bulletBody->body->ApplyLinearImpulseToCenter(b2Vec2(m_moveSpeed * direction.x * bulletBody->body->GetMass() , (m_moveSpeed * direction.y )* bulletBody->body->GetMass()), false);

	Vector3 bulletPos;
	bulletPos.z = this->GetPosition().z;
	bulletPos.x = this->bulletBody->body->GetPosition().x;
	bulletPos.y = this->bulletBody->body->GetPosition().y;

	this->SetPosition(bulletPos);
}

void CanonBullet::Update(float deltaTime)
{
	if (time == 1) {
		Fire(b2Vec2(sin(-m_rotation), cos(-m_rotation)));
		time++;
	}
	else {
		Vector3 bulletPos;
		bulletPos.z = this->GetPosition().z;
		bulletPos.x = this->bulletBody->body->GetPosition().x;
		bulletPos.y = this->bulletBody->body->GetPosition().y;

		this->SetPosition(bulletPos);
		time++;
	}
}
