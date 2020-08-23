#include "DeadlyBird.h"
#include"WorldManager.h"
#include"Singleton.h"
#include"SceneManager2D.h"
#include<math.h>
DeadlyBird::DeadlyBird(int id):Enemy(id)
{
}
DeadlyBird::~DeadlyBird()
{
}
void DeadlyBird::Update(float deltaTime)
{
	Vector3 thisPos = this->GetPosition(), playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	Vector2 pos2d, playerPos2D(playerPos.x, playerPos.y);
	pos2d.x = thisPos.x;pos2d.y = thisPos.y;
	Vector2 tmp = -(pos2d - playerPos2D);
	if (tmp.Length() < 10.0 && active == 0) {
		m_target = playerPos2D;
		active = 1;
	}
	if (active == 1) {
		b2Vec2 currentV = this->enemyBody->body->GetLinearVelocity();
		Vector2 vInVec2(currentV.x, currentV.y);
		float cos_angle = (vInVec2.y) / (vInVec2.Length());
		if (vInVec2.x>0) this->SetRotation(-acos(cos_angle));
		else
			this->SetRotation(acos(cos_angle));
		tmp.Normalize();
		tmp *= 2;
		Seek(tmp, vInVec2);
		enemyBody->body->ApplyLinearImpulseToCenter(m_attackVector, false);
	}
	thisPos.x = enemyBody->body->GetPosition().x;
	thisPos.y = enemyBody->body->GetPosition().y;
	this->SetPosition(thisPos);
}
void DeadlyBird::Seek(Vector2 target, Vector2 currentV)
{
	m_attackVector = b2Vec2(target.x - currentV.x, target.y - currentV.y);
	
}
void DeadlyBird::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(SPECIAL_ENEMY, x, y, width, height, 8);
}
