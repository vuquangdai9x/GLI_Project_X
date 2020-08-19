#include "GunBullet.h"
#include "SceneManager2D.h"
#include "WorldManager.h"
GunBullet::GunBullet(int id, float mass, float damage, float initSpeed): Bullet(id,mass,damage,initSpeed)
{}

GunBullet::GunBullet(int id, GunBullet & templateBullet)
	: Bullet(id,templateBullet)
{
}

GunBullet::~GunBullet()
{
}

void GunBullet::InitPhysics()
{
	if (m_mainTexture == NULL) {
		printf("[ERR] CannonBullet: MainTexure was not set\n");
		return;
	}
	float width = m_originSize.x * this->GetScale().x;
	float height = m_originSize.y * this->GetScale().y;
	this->bulletBody = Singleton<WorldManager>::GetInstance()->createRectagle(GUNBULLET, 1, 2, width, height);
	SetActiveBullet(false);
}

void GunBullet::Fire(Player* player, Vector2 startPosition, Vector2 direction)
{
	printf("fire\n");
	Vector2 normDirection = direction.Normalize();
	// move to startPosition and rotate
	SetActiveBullet(false);
	float rotation = acosf(normDirection.x);
	bulletBody->body->SetTransform(b2Vec2(startPosition.x, startPosition.y), rotation);
	SetActiveBullet(true);
	// set velocity
	bulletBody->body->SetLinearVelocity(b2Vec2(m_initSpeed * direction.x, m_initSpeed * direction.y));

	m_existTime = 5;
}

void GunBullet::SetActiveBullet(bool value)
{
	bulletBody->body->SetActive(value);
	SetActiveSprite(value);
}

void GunBullet::Update(float deltaTime)
{
	Bullet::Update(deltaTime);
	if (m_existTime > 0) {
		m_existTime -= deltaTime;
		if (m_existTime < 0) SetActiveBullet(false);
	}
}
