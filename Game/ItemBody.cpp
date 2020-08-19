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

	/*switch (m_type)
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
		
		break;
	}
	case GUNBULLET: {
		body->ApplyForce(b2Vec2(0.0f, GRAVITY * body->GetMass()), body->GetWorldCenter(), true);
		break;
	}
	}*/
	//b2Vec2 position = body->GetPosition();
	//float32 angle = body->GetAngle();

	//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

void ItemBody::SetGravityScale(float value)
{
	m_gravityScale = value;
}
