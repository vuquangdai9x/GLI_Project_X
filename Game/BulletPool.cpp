#include "BulletPool.h"

BulletPool::BulletPool(int id, int iCapacity): m_id(id), m_iCapacity(iCapacity)
{}

BulletPool::~BulletPool()
{
	delete[] m_aBullet;
}

Bullet* BulletPool::GetBullet()
{
	for (int i = 0;i < m_iCapacity;i++) {
		if ( ! m_aBullet[i]->CheckIsActiveSprite())
			return m_aBullet[i];
	}
	return NULL;
}
