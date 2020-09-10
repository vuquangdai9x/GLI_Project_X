#include "DeadlyBird.h"
#include"WorldManager.h"
#include"Singleton.h"
#include"SceneManager2D.h"
#include"EffectManager.h"
#include<math.h>
DeadlyBird::DeadlyBird(int id):Enemy(id)
{
	this->m_maxHP = this->m_HP = 50;
	this->m_damage = 90;
	this->m_score = 9;
}
DeadlyBird::~DeadlyBird()
{
}
void DeadlyBird::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
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
		enemyBody->body->ApplyLinearImpulseToCenter(m_attackVector, true);
	}
	thisPos.x = enemyBody->body->GetPosition().x;
	thisPos.y = enemyBody->body->GetPosition().y;
	this->SetPosition(thisPos);

	DWORD start;
	start = GetTickCount();
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			m_TakeDameTime = GetTickCount();
			this->m_HP -= user->m_receiveDamage;
			this->SetColor(0xffafff, 1);
			if (this->m_HP <= 0) {
				Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(GetPosition(), 12200);
				Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
				int score = Singleton<SceneManager2D>::GetInstance()->getPlayer()->getScore();
				Singleton<SceneManager2D>::GetInstance()->getPlayer()->setScore(score + this->m_score);
				this->enemyBody->setActive(false);
				delete this;
			}
		}
		else {
			if (start - m_TakeDameTime> m_animationTime)this->SetColor(0xffffff, 1);
		}
	}
	else if (start - m_TakeDameTime > m_animationTime) this->SetColor(0xffffff, 1);
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
	UserData* user = (UserData*)this->enemyBody->body->GetUserData();
	user->m_damage = this->m_damage;
}
