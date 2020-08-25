#include "RockyGolem.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include "FallingRockPool.h"
#include "SimpleGun.h"

RockyGolem::RockyGolem(int id) :Enemy(id)
{
	this->createWeapon();

	this->m_maxHP = this->m_HP = 100;
}

RockyGolem::~RockyGolem()
{
}

void RockyGolem::Update(float deltaTime)
{
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	Vector2 target;
	target.x = (playerPos.x - 1) + scale * 2;
	target.y = (playerPos.y - 1) + scale * 2;
	time++;
	if (time % 200 == 0) {
		this->rockyGolemGun->Fire(this, target);
	}

	Vector3 rockyGolemPos = this->GetPosition();
	rockyGolemPos.x = this->enemyBody->body->GetPosition().x;
	rockyGolemPos.y = this->enemyBody->body->GetPosition().y;
	this->SetPosition(rockyGolemPos);

	Enemy::takeDamage();
}

void RockyGolem::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(ENEMY, x, y, width, height, 200);
}

void RockyGolem::createWeapon()
{
	FallingRock* templateFallingRock = new FallingRock(-1, 3.0, 1.0, 7.0, 15.0, 3);
	templateFallingRock->Init(this->GetPosition(), 0, Vector2(1.0, 1.0), 0xffffff, 1.0, 0, 10);
	FallingRockPool *pool = new FallingRockPool(3, -1, 10, templateFallingRock);
	this->rockyGolemGun = new SimpleGun(6, "Rocky", 0, 0, 0, 0, 0, 45 * M_PI / 180, 3);
	this->rockyGolemGun->BindBulletPool(pool);
}
