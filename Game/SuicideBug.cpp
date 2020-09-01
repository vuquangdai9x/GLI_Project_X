#include "SuicideBug.h"
#include"WorldManager.h"
#include"Singleton.h"
#include"SceneManager2D.h"
#include<math.h>
#include "SoundManager.h"
SuicideBug::SuicideBug(int id) :Enemy(id)
{
	this->m_maxHP = this->m_HP = 20;
	this->m_damage = 50;
}

SuicideBug::~SuicideBug()
{
}

void SuicideBug::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	Vector3 thisPos = this->GetPosition(), playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	Vector2 pos2d,playerPos2D(playerPos.x,playerPos.y);
	pos2d.x = thisPos.x;pos2d.y = thisPos.y;
	Vector2 tmp = -(pos2d - playerPos2D);
	if (tmp.Length() < 15.0 && active==0) {
		Singleton<SoundManager>::GetInstance()->Enemy(SoundManager::E_BUG);
		m_target = playerPos2D;
		active = 1;
		m_attackVector = b2Vec2((m_target - pos2d).x  , (m_target - pos2d).y );
	}
	if (active == 0) {
		float cos_angle = (tmp.y) / (tmp.Length());
		if(pos2d.x< playerPos2D.x) this->SetRotation(-acos(cos_angle));
		else 
		this->SetRotation(acos(cos_angle));
	}
	else {
		enemyBody->body->ApplyLinearImpulseToCenter(m_attackVector, false);
	}
	 m_activeTime += deltaTime* active;
	 if (m_activeTime > 3) {
		 //in avtive or smt;
	 }
	 thisPos.x = enemyBody->body->GetPosition().x;
	 thisPos.y = enemyBody->body->GetPosition().y;

	 this->SetPosition(thisPos);

	 Enemy::takeDamage();
}

void SuicideBug::SetIgnore()
{
}

void SuicideBug::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(SPECIAL_ENEMY, x, y, width, height, 8);
	UserData* user = (UserData*)this->enemyBody->body->GetUserData();
	user->m_damage = this->m_damage;
}
