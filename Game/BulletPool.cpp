#include "BulletPool.h"

Bullet* BulletPool::GetBulletFromPool()
{
	for (int i = 0;i < m_listBullet.size();i++) {
		if (!m_listBullet[i]->CheckIsActiveSprite()) {
			return m_listBullet[i];
		}
	}
	CreateNewBullet();
	return m_listBullet.back();
}

BulletPool::BulletPool(int id, int iCapacity): m_id(id), m_iCapacity(iCapacity), m_iCount(0)
{}

BulletPool::~BulletPool()
{
}

Bullet* BulletPool::GetBullet()
{
	if (m_iCapacity > 0 && m_iCount <= 0) return NULL; //capacity<0: infinite bullet
	m_iCount--;
	return GetBulletFromPool();
}

int BulletPool::GetId()
{
	return m_id;
}

int BulletPool::GetCountBullet()
{
	return m_iCount;
}

int BulletPool::GetCapacity()
{
	return m_iCapacity;
}

void BulletPool::AddBullet(int iAmount)
{
	m_iCount += iAmount;
	if (m_iCount < 0) m_iCount = 0;
	else if (m_iCount > m_iCapacity) m_iCount = m_iCapacity;
}
