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
		body->ApplyForce(b2Vec2(0.0f, GRAVITY), body->GetWorldCenter(), true);
		if (!firstTime) {
			firstTime++;
			body->ApplyLinearImpulseToCenter(b2Vec2(0.0, -(Y_FORCE)), true);
			break;
		}
		if (firstTime == 1) {
			firstTime++;
			body->ApplyLinearImpulseToCenter(b2Vec2(0.0, (Y_FORCE)), true);
			break;
		}
		float force_y = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S));
		float force_x = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A));
		if (pre_KeyY != force_y) body->ApplyLinearImpulseToCenter(b2Vec2(0.0, (force_y - pre_KeyY) * (Y_FORCE)), true);
		if (pre_KeyX != force_x) body->ApplyLinearImpulseToCenter(b2Vec2((force_x - pre_KeyX) * X_FORCE, 0.0), true);
		pre_KeyX = force_x, pre_KeyY = force_y;
		break;
	}
	case OBSTACLE: {
		body->ApplyForce(b2Vec2(0.0f, GRAVITY), body->GetWorldCenter(), true);
		break;
	}
	case STATIC_OBJECT: {
		break;
	}
	}
	//b2Vec2 position = body->GetPosition();
	//float32 angle = body->GetAngle();

	//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}
