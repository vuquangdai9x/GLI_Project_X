#include "VampireBat.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"

VampireBat::VampireBat(int id, Vector2 startPos, Vector2 endPos) : Enemy(id)
{
	this->startPos = startPos;
	this->endPos =  endPos;

	this->m_velocity = b2Vec2(0, 0);
	this->m_radius = 10;
	this->m_moveSpeed = 500;
	this->m_maxSpeed = 10;
	this->m_maxforce = 0.2;
	this->direction = true;
	this->m_active = false;
	basic_Dmg = 5;
	this->m_maxHP = this->m_HP = 5;
}

VampireBat::~VampireBat()
{
}

void VampireBat::Update(float deltaTime)
{
	Vector3 vampireBatPos = this->GetPosition();
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	if (!m_active) {
		if (checkPlayer()) {
			Vector2 desire(playerPos.x - vampireBatPos.x, playerPos.y - vampireBatPos.y);
			desire.Normalize();
			b2Vec2 m_attackVector(desire.x * m_moveSpeed, desire.y * m_moveSpeed);
			enemyBody->body->ApplyLinearImpulseToCenter(m_attackVector, false);
			this->m_active = true;
		}
		else {
			if (direction) {
				Vector2 desired(endPos.x - vampireBatPos.x, endPos.y - vampireBatPos.y);
				desired.Normalize();
				desired = Vector2(desired.x * m_maxSpeed * 0.05, desired.y * m_maxSpeed * 0.05);
				Vector2 steer(desired.x - m_velocity.x, desired.y - m_velocity.y);
				if (abs(steer.x) > m_maxforce) {
					steer.x = (steer.x > 0) ? m_maxforce : -m_maxforce;
				}
				if (abs(steer.y) > m_maxforce) {
					steer.y = (steer.y > 0) ? m_maxforce : -m_maxforce;
				}

				this->m_velocity = b2Vec2(this->m_velocity.x + steer.x, this->m_velocity.y + steer.y);
				if (abs(this->m_velocity.x) > m_maxSpeed) {
					if (this->m_velocity.x > 0) this->m_velocity.x = m_maxSpeed;
					else this->m_velocity.x = -m_maxSpeed;
				}
				if (abs(this->m_velocity.y) > m_maxSpeed) {
					if (this->m_velocity.y > 0) this->m_velocity.y = m_maxSpeed;
					else this->m_velocity.y = -m_maxSpeed;
				}
				this->enemyBody->body->SetLinearVelocity(b2Vec2(this->m_velocity.x * 10, this->m_velocity.y * 10));
				if (endPos.x - 0.5 <= vampireBatPos.x && vampireBatPos.x <= endPos.x + 0.5 &&
					endPos.y - 0.5 <= vampireBatPos.y && vampireBatPos.y <= endPos.y + 0.5) {
					direction = false;
				}
			}
			else {
				Vector2 desired(startPos.x - vampireBatPos.x, startPos.y - vampireBatPos.y);
				desired.Normalize();
				desired = Vector2(desired.x * m_maxSpeed * 0.05, desired.y * m_maxSpeed * 0.05);
				Vector2 steer(desired.x - m_velocity.x, desired.y - m_velocity.y);
				if (abs(steer.x) > m_maxforce) {
					steer.x = (steer.x > 0) ? m_maxforce : -m_maxforce;
				}
				if (abs(steer.y) > m_maxforce) {
					steer.y = (steer.y > 0) ? m_maxforce : -m_maxforce;
				}

				this->m_velocity = b2Vec2(this->m_velocity.x + steer.x, this->m_velocity.y + steer.y);
				if (abs(this->m_velocity.x) > m_maxSpeed) {
					if (this->m_velocity.x > 0) this->m_velocity.x = m_maxSpeed;
					else this->m_velocity.x = -m_maxSpeed;
				}
				if (abs(this->m_velocity.y) > m_maxSpeed) {
					if (this->m_velocity.y > 0) this->m_velocity.y = m_maxSpeed;
					else this->m_velocity.y = -m_maxSpeed;
				}
				this->enemyBody->body->SetLinearVelocity(b2Vec2(this->m_velocity.x * 10, this->m_velocity.y * 10));
				if (startPos.x - 0.5 <= vampireBatPos.x && vampireBatPos.x <= startPos.x + 0.5 &&
					startPos.y - 0.5 <= vampireBatPos.y && vampireBatPos.y <= startPos.y + 0.5) {
					direction = true;
				}
			}
		}
	} 
	vampireBatPos.x = this->enemyBody->body->GetPosition().x;
	vampireBatPos.y = this->enemyBody->body->GetPosition().y;
	this->SetPosition(vampireBatPos);

	Enemy::takeDamage();
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
	user->m_damage = basic_Dmg;
}

bool VampireBat::checkPlayer()
{
	Vector3 vampireBatPos = this->GetPosition();
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	if (vampireBatPos.y > playerPos.y) {
		Vector2 dis(vampireBatPos.x - playerPos.x, vampireBatPos.y - playerPos.y);
		if (dis.Length() < m_radius) {
			return true;
		}
		else return false;
	}
	return false;
}
