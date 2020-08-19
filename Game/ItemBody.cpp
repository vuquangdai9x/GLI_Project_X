#include "ItemBody.h"
#include"Singleton.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
ItemBody::ItemBody(int type, float x, float y)
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
	switch (m_type)
	{
	case PLAYER: {
		body->ApplyForce(b2Vec2(0.0f, GRAVITY*body->GetMass()), body->GetWorldCenter(), true);
		
		break;
	}
	case CANONBULLET:
	case OBSTACLE: {
		
		break;
	}
	case ENEMY: {
		body->ApplyForce(b2Vec2(0.0f, GRAVITY * body->GetMass()), body->GetWorldCenter(), false);
		break;
	}
	case GUNBULLET: {
		body->ApplyForce(b2Vec2(0.0f, GRAVITY * body->GetMass()), body->GetWorldCenter(), true);
		break;
	}
	}
	//b2Vec2 position = body->GetPosition();
	//float32 angle = body->GetAngle();

	//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}
