#include "FloatingFish.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include "EffectManager.h"
#include "Misson.h"
FloatingFish::FloatingFish(int id, int numOfTarget, b2Vec2 target[]):Enemy(id)
{
	this->numOfTarget = numOfTarget;
	this->target = new b2Vec2[this->numOfTarget];
	for (int i = 0; i < numOfTarget; i++) {
		this->target[i] = target[i];
	}
	this->currentTarget = 0;
	this->m_moveSpeed = 4;
	this->m_maxforce = 0.1;
	this->velocity = b2Vec2(0, 0);
	this->direction = true;
	this->m_damage = 25;
	this->m_maxHP = this->m_HP = 100;
}

FloatingFish::FloatingFish(int id):Enemy(id)
{
}

FloatingFish::~FloatingFish()
{
}

void FloatingFish::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	Vector3 floatingFishPos = this->GetPosition();
	if (direction) {
		Vector2 desired (target[currentTarget+1].x - floatingFishPos.x, target[currentTarget+1].y - floatingFishPos.y);
		desired.Normalize();
		desired = Vector2(desired.x * m_moveSpeed * 0.05, desired.y * m_moveSpeed * 0.05);
		Vector2 steer(desired.x - velocity.x, desired.y - velocity.y);
		if (abs(steer.x) > m_maxforce) {
			steer.x = (steer.x > 0) ? m_maxforce : -m_maxforce;
		}
		if (abs(steer.y) > m_maxforce) {
			steer.y = (steer.y > 0) ? m_maxforce : -m_maxforce;
		}

		this->velocity = b2Vec2(this->velocity.x + steer.x, this->velocity.y + steer.y);
		if (abs(this->velocity.x) > m_moveSpeed) {
			if (this->velocity.x > 0) this->velocity.x = m_moveSpeed;
			else this->velocity.x = -m_moveSpeed;
		}
		if (abs(this->velocity.y) > m_moveSpeed) {
			if (this->velocity.y > 0) this->velocity.y = m_moveSpeed;
			else this->velocity.y = -m_moveSpeed;
		}
		this->enemyBody->body->SetLinearVelocity(b2Vec2(this->velocity.x * 10, this->velocity.y * 10));
		floatingFishPos.x = this->enemyBody->body->GetPosition().x;
		floatingFishPos.y = this->enemyBody->body->GetPosition().y;
		//this->enemyBody->body->GetPosition().x = floatingFishPos.x;
		this->SetPosition(floatingFishPos);
		if (floatingFishPos.x >= target[currentTarget + 1].x - 0.5 && floatingFishPos.x <= target[currentTarget + 1].x + 0.5 && 
			floatingFishPos.y >= target[currentTarget + 1].y - 0.5 && floatingFishPos.y <= target[currentTarget + 1].y + 0.5) {
			currentTarget++;
			this->velocity = b2Vec2(0, 0);
			if (currentTarget == numOfTarget - 1) direction = false;
		}
	}
	else {
		Vector2 desired(target[currentTarget - 1].x - floatingFishPos.x, target[currentTarget - 1].y - floatingFishPos.y);
		desired.Normalize();
		desired = Vector2(desired.x * m_moveSpeed * 0.05, desired.y * m_moveSpeed * 0.05);
		Vector2 steer(desired.x - velocity.x, desired.y - velocity.y);
		if (abs(steer.x) > m_maxforce) {
			steer.x = (steer.x > 0) ? m_maxforce : -m_maxforce;
		}
		if (abs(steer.y) > m_maxforce) {
			steer.y = (steer.y > 0) ? m_maxforce : -m_maxforce;
		}

		this->velocity = b2Vec2(this->velocity.x + steer.x, this->velocity.y + steer.y);
		if (abs(this->velocity.x) > m_moveSpeed) {
			if (this->velocity.x > 0) this->velocity.x = m_moveSpeed;
			else this->velocity.x = -m_moveSpeed;
		}
		if (abs(this->velocity.y) > m_moveSpeed) {
			if (this->velocity.y > 0) this->velocity.y = m_moveSpeed;
			else this->velocity.y = -m_moveSpeed;
		}
		this->enemyBody->body->SetLinearVelocity(b2Vec2(this->velocity.x * 10, this->velocity.y * 10));
		floatingFishPos.x = this->enemyBody->body->GetPosition().x;
		floatingFishPos.y = this->enemyBody->body->GetPosition().y;

		this->SetPosition(floatingFishPos);
		if (floatingFishPos.x >= target[currentTarget - 1].x - 0.5 && floatingFishPos.x <= target[currentTarget - 1].x + 0.5 &&
			floatingFishPos.y >= target[currentTarget - 1].y - 0.5 && floatingFishPos.y <= target[currentTarget - 1].y + 0.5) {
			currentTarget--;
			this->velocity = b2Vec2(0, 0);
			if (currentTarget == 0) direction = true;
		}
	}

	if (this->enemyBody->body->GetLinearVelocity().x * m_scale.x < 0) {
		SetScale(Vector2(m_scale.x * -1, m_scale.y));
	}

	//Enemy::takeDamage();
	//printf("%d\n", this->m_HP);
	DWORD start;
	start = GetTickCount();
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			m_TakeDameTime = GetTickCount();
			this->m_HP -= user->m_receiveDamage;
			this->SetColor(0xffafff, 1);
			if (this->m_HP <= 0) {
				Misson* misson = Singleton<SceneManager2D>::GetInstance()->getPlayer()->getMisson();
				for (int i = 0; i < 3; i++) {
					if (misson->getListMisson()[i] == 1 && misson->getEnemyType() == 0) {
						misson->countKill();
					}
				}
				Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(GetPosition(), 12100);
				Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
				this->enemyBody->setActive(false);
				delete this;
			}
		}
		else {
			if (start - m_TakeDameTime > m_animationTime)this->SetColor(0xffffff, 1);
		}
	}
	else if (start - m_TakeDameTime > m_animationTime) this->SetColor(0xffffff, 1);
}

void FloatingFish::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createFloating(ENEMY, x, y, width, height,8);
	UserData* user = (UserData*)this->enemyBody->body->GetUserData();
	user->m_damage = this->m_damage;
}
