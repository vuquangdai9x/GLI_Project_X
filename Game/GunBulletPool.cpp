#include "GunBulletPool.h"
#include "SceneManager2D.h"

GunBulletPool::GunBulletPool(int id, int iCapacity, GunBullet& templateBullet)
	: BulletPool(id, iCapacity)
{
	m_aBullet = new Bullet * [iCapacity];
	for (int i = 0;i < iCapacity;i++) {
		// no need id, because we can access object from pool. So I set id=-1
		m_aBullet[i] = new GunBullet(-1, templateBullet);
		m_aBullet[i]->InitPhysics();
		Singleton<SceneManager2D>::GetInstance()->AddObject(m_aBullet[i]);
	}
}
