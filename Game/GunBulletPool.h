#pragma once
#include "BulletPool.h"
#include "GunBullet.h"

class GunBulletPool : public BulletPool {
protected:
	GunBullet* m_templateBullet;
	void CreateNewBullet();
public:
	GunBulletPool(int id, int iCapacity, int iInitPoolAmount, GunBullet* templateBullet);
	virtual ~GunBulletPool();
};