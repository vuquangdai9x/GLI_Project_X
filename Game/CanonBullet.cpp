#include "CanonBullet.h"
#include "SceneManager2D.h"
#include "WorldManager.h"
CanonBullet::CanonBullet(int id, float mass, float damage, float initSpeed) : Bullet(id, mass, damage, initSpeed)
{}

CanonBullet::CanonBullet(int id, CanonBullet & templateBullet)
	: Bullet(id, templateBullet)
{
}

CanonBullet::~CanonBullet()
{
}

void CanonBullet::InitBullet()
{
	if (m_mainTexture == NULL) {
		printf("[ERR] CannonBullet: MainTexure was not set\n");
		return;
	}
	float width = m_originSize.x * this->GetScale().x;
	float height = m_originSize.y * this->GetScale().y;
	this->bulletBody = Singleton<WorldManager>::GetInstance()->createRectagle(CANONBULLET, 0, 0, width, height);
	SetActiveBullet(false);
}

void CanonBullet::Fire(Player* player, Vector2 startPosition, Vector2 direction)
{
	Vector2 normDirection = direction.Normalize();
	// move to startPosition and rotate
	float rotation = acosf(normDirection.x);
	bulletBody->body->SetTransform(b2Vec2(startPosition.x, startPosition.y), rotation);
	
	Vector3 pos;
	pos.x = startPosition.x;
	pos.y = startPosition.y;
	pos.z = GetPosition().z;
	this->SetPosition(pos);
	this->SetRotation(rotation);

	SetActiveBullet(true);
	// set velocity
	bulletBody->body->SetLinearVelocity(b2Vec2(m_initSpeed * direction.x, m_initSpeed * direction.y));
}

void CanonBullet::SetActiveBullet(bool value)
{
	bulletBody->body->SetActive(value);
	SetActiveSprite(value);
}