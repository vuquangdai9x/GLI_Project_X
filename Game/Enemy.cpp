#include "Enemy.h"
#include "UserData.h"

Enemy::Enemy(int id):Sprite(id)
{

}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
}

void Enemy::createBox2D()
{
}

void Enemy::takeDamage()
{
	
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			this->m_HP -= user->m_receiveDamage;
			if (this->m_HP <= 0) {
				
			}
		}
	}
}
