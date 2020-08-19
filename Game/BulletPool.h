#pragma once
#include "Bullet.h"
class BulletPool {
protected:
	int m_id;
	int m_iCapacity;
	Bullet** m_aBullet;
public:
	BulletPool(int id, int iCapacity);
	~BulletPool();
	Bullet* GetBullet();
};