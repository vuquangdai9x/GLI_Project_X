#include "RockyGolem.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include "FallingRockPool.h"
#include "SimpleGun.h"
#include "EffectManager.h"

RockyGolem::RockyGolem(int id) :Enemy(id)
{
	this->createWeapon();

	this->m_maxHP = this->m_HP = 100;
	this->m_damage = 25;
	this->m_score = 11;

}

RockyGolem::~RockyGolem()
{
	delete rockyGolemGun;
}

void RockyGolem::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	Vector2 target;
	target.x = (playerPos.x - 1) + scale * 2;
	target.y = (playerPos.y - 1) + scale * 2;
	time++;
	if (time % 200 == 0) {
		this->rockyGolemGun->Fire(0, this, target);
	}

	Vector3 rockyGolemPos = this->GetPosition();
	rockyGolemPos.x = this->enemyBody->body->GetPosition().x;
	rockyGolemPos.y = this->enemyBody->body->GetPosition().y;
	this->SetPosition(rockyGolemPos);

	//Enemy::takeDamage();
	DWORD start;
	start = GetTickCount();
	UserData* user = (UserData*)enemyBody->body->GetUserData();
	if (user->IsCollison > 0) {
		if (user->m_typeB == PLAYERBULLET) {
			m_TakeDameTime = GetTickCount();
			this->m_HP -= user->m_receiveDamage;
			this->SetColor(0xffafff, 1);
			if (this->m_HP <= 0) {
				Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(GetPosition(), 12400);
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

void RockyGolem::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	enemyBody = Singleton<WorldManager>::GetInstance()->createRectagle(SPECIAL_ENEMY, x, y, width, height, 200);
	enemyBody->SetGravityScale(0.0);
	enemyBody->body->GetFixtureList()[0].SetFriction(10.0);
	UserData* user = (UserData*)this->enemyBody->body->GetUserData();
	user->m_damage = this->m_damage;
}

void RockyGolem::createWeapon()
{
	FallingRock* templateFallingRock = new FallingRock(-1, 3.0, 1.0, 7.0, 15.0, 8);
	templateFallingRock->Init(this->GetPosition(), 0, Vector2(1.0, 1.0), 0xffffff, 1.0, 0, 20301);
	FallingRockPool *pool = new FallingRockPool(3, -1, 10, templateFallingRock);
	this->rockyGolemGun = new SimpleGun(6, "Rocky", 0, 0, 0, 0, 0, 45 * M_PI / 180, 3);
	this->rockyGolemGun->BindBulletPool(pool);
}
