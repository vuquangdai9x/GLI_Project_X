#include "SuicideBug.h"
#include"WorldManager.h"
#include"Singleton.h"
#include"SceneManager2D.h"
#include"EffectManager.h"
#include<math.h>
#include "SoundManager.h"

SuicideBug::SuicideBug(int id) :Enemy(id)
{
	this->m_maxHP = this->m_HP = 20;
	this->m_damage = 50;
	this->m_score = 11;
}

SuicideBug::~SuicideBug()
{
}

void SuicideBug::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	this->SetUseAnimation(false);
	Vector3 thisPos = this->GetPosition(), playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	Vector2 pos2d, playerPos2D(playerPos.x, playerPos.y);
	pos2d.x = thisPos.x; pos2d.y = thisPos.y;
	Vector2 tmp = -(pos2d - playerPos2D);
	if (tmp.Length() < 15.0&&active == 0) {
		this->SetUseAnimation(true);
		active = 1;

	}
	if (active == 1) {
		this->SetUseAnimation(true);
		float cos_angle = (tmp.y) / (tmp.Length());
		if (pos2d.x< playerPos2D.x) this->SetRotation(-acos(cos_angle));
		else
			this->SetRotation(acos(cos_angle));

		
			
		
	}
	else if (active == 2) {
		this->SetUseAnimation(true);
		enemyBody->body->ApplyLinearImpulseToCenter(m_attackVector, true);
	}
	m_activeTime += deltaTime* active;
	if (m_activeTime > 0.5&& active==1) {
		Singleton<SoundManager>::GetInstance()->Enemy(SoundManager::E_BUG);
		m_target = playerPos2D;
		active = 2;
		m_attackVector = b2Vec2((m_target - pos2d).x, (m_target - pos2d).y);
	}
	thisPos.x = enemyBody->body->GetPosition().x;
	thisPos.y = enemyBody->body->GetPosition().y;

	this->SetPosition(thisPos);
	DWORD start;
	start = GetTickCount();
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYER) {
			Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(GetPosition(), 12100);
			Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
			this->enemyBody->setActive(false);
			delete this;
		}
		else {
			Enemy::takeDamage();
		}
	}
	
	
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