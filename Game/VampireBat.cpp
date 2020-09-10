#include "VampireBat.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include"EffectManager.h"
VampireBat::VampireBat(int id, Vector2 centerPos, float m_range) : Enemy(id)
{
	this->centerPos = centerPos;
	this->m_range = m_range;

	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	this->target.x = (centerPos.x - m_range) + scale * 2 * m_range;
	scale = rand() / (float)RAND_MAX;
	this->target.y = (centerPos.y - m_range) + scale * 2 * m_range;

	this->m_velocity = b2Vec2(0, 0);
	this->m_radius = 10;
	this->m_moveSpeed = 10;
	this->m_maxSpeed = 500;
	this->m_maxforce = 0.1;

	this->m_active = false;

	this->m_damage = 1;
	this->m_maxHP = this->m_HP = 15;
	this->m_score = 13;
}

VampireBat::~VampireBat()
{
}

void VampireBat::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	Vector3 vampireBatPos = this->GetPosition();
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);

	move(this->m_moveSpeed);
	
	if (target.x - 0.5 <= vampireBatPos.x && vampireBatPos.x <= target.x + 0.5 &&
		target.y - 0.5 <= vampireBatPos.y && vampireBatPos.y <= target.y + 0.5) {

		float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
		this->target.x = (centerPos.x - m_range) + scale * 2 * m_range;
		scale = rand() / (float)RAND_MAX;
		this->target.y = (centerPos.y - m_range) + scale * 2 * m_range;
		this->m_active = false;
	}
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYER) {
			float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
			this->target.x = (centerPos.x - m_range) + scale * 2 * m_range;
			scale = rand() / (float)RAND_MAX;
			this->target.y = (centerPos.y - m_range) + scale * 2 * m_range;
			this->m_active = true;
		}
	}
	if (checkPlayer() && !m_active) {
		this->target.x = playerPos.x;
		this->target.y = playerPos.y;
		this->move(m_maxSpeed);
	}
	
	
	vampireBatPos.x = this->enemyBody->body->GetPosition().x;
	vampireBatPos.y = this->enemyBody->body->GetPosition().y;
	this->SetPosition(vampireBatPos);

	
	DWORD start;
	start = GetTickCount();
//	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			m_TakeDameTime = GetTickCount();
			this->m_HP -= user->m_receiveDamage;
			this->SetColor(0xffafff, 1);
			if (this->m_HP <= 0) {
				Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(GetPosition(), 12300);
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

void VampireBat::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(SPECIAL_ENEMY, x, y, width, height, 6);
	enemyBody->SetGravityScale(0);
	UserData* user = (UserData*)this->enemyBody->body->GetUserData();
	user->m_damage = m_damage;
}

bool VampireBat::checkPlayer()
{
	Vector3 vampireBatPos = this->GetPosition();
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	Vector2 dis(vampireBatPos.x - playerPos.x, vampireBatPos.y - playerPos.y);
	if (dis.Length() < m_radius) {
		return true;
	}
	return false;
}

void VampireBat::move(float moveSpeed)
{
	Vector3 vampireBatPos = this->GetPosition();

	Vector2 desired(target.x - vampireBatPos.x, target.y - vampireBatPos.y);
	desired.Normalize();
	desired = Vector2(desired.x * moveSpeed * 0.05, desired.y * moveSpeed * 0.05);
	Vector2 steer(desired.x - m_velocity.x, desired.y - m_velocity.y);
	if (abs(steer.x) > m_maxforce) {
		steer.x = (steer.x > 0) ? m_maxforce : -m_maxforce;
	}
	if (abs(steer.y) > m_maxforce) {
		steer.y = (steer.y > 0) ? m_maxforce : -m_maxforce;
	}

	this->m_velocity = b2Vec2(this->m_velocity.x + steer.x, this->m_velocity.y + steer.y);
	if (abs(this->m_velocity.x) > moveSpeed) {
		if (this->m_velocity.x > 0) this->m_velocity.x = moveSpeed;
		else this->m_velocity.x = -moveSpeed;
	}
	if (abs(this->m_velocity.y) > moveSpeed) {
		if (this->m_velocity.y > 0) this->m_velocity.y = moveSpeed;
		else this->m_velocity.y = -moveSpeed;
	}
	this->enemyBody->body->SetLinearVelocity(b2Vec2(this->m_velocity.x * 10, this->m_velocity.y * 10));
}
