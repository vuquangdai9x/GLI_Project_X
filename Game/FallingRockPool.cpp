#include "FallingRockPool.h"
#include "SceneManager2D.h"

void FallingRockPool::CreateNewBullet()
{
	FallingRock* bullet = new FallingRock(-1, *m_templateBullet);
	bullet->CreatePhysicsBody();
	bullet->SetActiveBullet(false);
	m_listBullet.push_back(bullet);
	Singleton<SceneManager2D>::GetInstance()->AddObject(bullet);
	//printf("[msg] FallingRockPool: Created and add a bullet to pool\n");
}

FallingRockPool::FallingRockPool(int id, int iCapacity, int iInitPoolAmount, FallingRock* templateBullet)
	: BulletPool(id, iCapacity), m_templateBullet(templateBullet) 
{
	FallingRock* bullet;
	for (int i = 0; i < iInitPoolAmount; i++) {
		// no need id, because we can access object from pool. So I set id=-1
		bullet = new FallingRock(-1, *templateBullet);
		bullet->CreatePhysicsBody();
		bullet->SetActiveBullet(false);
		m_listBullet.push_back(bullet);
		Singleton<SceneManager2D>::GetInstance()->AddObject(bullet);
	}
	printf("[msg] FallingRockPool: Created %d bullet in pool\n", iInitPoolAmount);
}

FallingRockPool::~FallingRockPool()
{
}
