#include "Enemy.h"
#include "UserData.h"
#include"Singleton.h"
#include"SceneManager2D.h"
#include "EffectManager.h"
Enemy::Enemy(int id):Sprite(id)
{

}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	Sprite::Update(deltaTime);
}

void Enemy::createBox2D()
{
}

void Enemy::takeDamage()
{
	DWORD start;
	start = GetTickCount();
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			m_TakeDameTime = GetTickCount();
			this->m_HP -= user->m_receiveDamage;
			this->SetColor(0xffafff, 1);
			if (this->m_HP <= 0) {
				Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
				this->enemyBody->setActive(false);
				delete this;
			}
		}
		else {
			if(start- m_TakeDameTime> m_animationTime)this->SetColor(0xffffff, 1);
		}
	}
	else if (start - m_TakeDameTime > m_animationTime) this->SetColor(0xffffff, 1);
}
