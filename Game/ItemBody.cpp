#include "ItemBody.h"
#include"Singleton.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
ItemBody::ItemBody(int type, float x, float y): m_gravityScale(0.0)
{
	m_type = type;
	m_x = x;
	m_y = y;
}

ItemBody::~ItemBody()
{
}

void ItemBody::Update(float deltaTime)
{
	body->ApplyForce(b2Vec2(0.0f, -GRAVITY * m_gravityScale * body->GetMass()), body->GetWorldCenter(), true);
}

void ItemBody::SetGravityScale(float value)
{
	m_gravityScale = value;
}

void ItemBody::setActive(bool active)
{
	m_active = active;
}
