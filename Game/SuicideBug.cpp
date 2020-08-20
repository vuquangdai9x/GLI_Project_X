#include "SuicideBug.h"
#include"WorldManager.h"
#include"Singleton.h"
SuicideBug::SuicideBug(int id) :Enemy(id)
{

}

void SuicideBug::Update(float deltaTime)
{
	Vector3 thisPos = this->GetPosition();
	Vector2 pos2d;
	pos2d.x = thisPos.x;pos2d.y = thisPos.y;
	if ((pos2d - m_target).Length() < 30.0&&active==0) {
		enemyBody->body->ApplyLinearImpulseToCenter(b2Vec2((m_target - pos2d).x, (m_target - pos2d).y), false);
		active = 1;
		

	}
	 m_activeTime += deltaTime* active;
	 if (m_activeTime > 3) {

	 }
	 thisPos.x = enemyBody->body->GetPosition().x;
	 thisPos.y = enemyBody->body->GetPosition().y;
	// enemyBody->body->SetLinearVelocity(b2Vec2(0, -10));

	 this->SetPosition(thisPos);
}

void SuicideBug::SetIgnore()
{
	b2Filter filter = b2Filter();
	filter.categoryBits = 0;
	filter.maskBits = 0;
	filter.groupIndex = -1;
	this->enemyBody->body->GetFixtureList()[0].SetFilterData(filter);
}

void SuicideBug::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(ENEMY, x, y, width, height, 8);
}
