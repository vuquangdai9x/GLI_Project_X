#include "GunBulletPool.h"
#include "SceneManager2D.h"

void GunBulletPool::CreateNewBullet()
{
	GunBullet* bullet = new GunBullet(-1, *m_templateBullet);
	bullet->CreatePhysicsBody();
	bullet->SetActiveBullet(false);
	m_listBullet.push_back(bullet);
	Singleton<SceneManager2D>::GetInstance()->AddObject(bullet);
	//printf("[msg] GunBulletPool: Created and add a bullet to pool\n");
}

GunBulletPool::GunBulletPool(int id, int iCapacity, int iInitPoolAmount, GunBullet* templateBullet) 
	: BulletPool(id,iCapacity), m_templateBullet(templateBullet)
{
	GunBullet* bullet;
	for (int i = 0;i < iInitPoolAmount;i++) {
		// no need id, because we can access object from pool. So I set id=-1
		bullet = new GunBullet(-1, *templateBullet);
		bullet->CreatePhysicsBody();
		bullet->SetActiveBullet(false);
		m_listBullet.push_back(bullet);
		Singleton<SceneManager2D>::GetInstance()->AddObject(bullet);
	}
	printf("[msg] GunBulletPool: Created %d bullet in pool\n", iInitPoolAmount);
}

GunBulletPool::~GunBulletPool()
{
}
