#include "GunBullet.h"
#include "SceneManager2D.h"
#include "WorldManager.h"

GunBullet::GunBullet(int id, float mass, float gravityScale, float damage, float initSpeed, float existTime)
	: Bullet(id), 
	m_mass(mass), m_gravityScale(gravityScale), 
	m_damage(damage), m_initSpeed(initSpeed), 
	m_existTime(existTime), m_timeCounter(-1)
{}

GunBullet::GunBullet(int id, GunBullet & templateBullet)
	:GunBullet(id,templateBullet.m_mass,templateBullet.m_gravityScale,templateBullet.m_damage,templateBullet.m_initSpeed,templateBullet.m_existTime)
{
	Init(templateBullet); // init sprite props
}

GunBullet::~GunBullet()
{
}

void GunBullet::CreatePhysicsBody()
{
	if (m_mainTexture == NULL) {
		printf("[ERR] CannonBullet: MainTexure was not set\n");
		return;
	}
	float width = m_originSize.x * this->GetScale().x;
	float height = m_originSize.y * this->GetScale().y;
	this->m_bulletBody = Singleton<WorldManager>::GetInstance()->createRectagle(GUNBULLET, 1, 2, width, height);
	this->m_bulletBody->SetGravityScale(m_gravityScale);
}

void GunBullet::Fire(Player* player, Vector2 startPosition, Vector2 direction)
{
	printf("fire\n");
	Vector2 normDirection = direction.Normalize();
	// move to startPosition and rotate
	float rotation = acosf(normDirection.x);
	m_bulletBody->body->SetTransform(b2Vec2(startPosition.x, startPosition.y), rotation);
	SetActiveBullet(true);
	// set velocity
	m_bulletBody->body->SetLinearVelocity(b2Vec2(m_initSpeed * direction.x, m_initSpeed * direction.y));

	m_timeCounter = m_existTime;
}

void GunBullet::Update(float deltaTime)
{
	Bullet::Update(deltaTime);
	if (m_timeCounter > 0) {
		m_timeCounter -= deltaTime;
		if (m_timeCounter < 0) SetActiveBullet(false);
	}
}
