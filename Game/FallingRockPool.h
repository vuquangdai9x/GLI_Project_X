#pragma once
#include "BulletPool.h"
#include "FallingRock.h"
class FallingRockPool : public BulletPool
{
protected:
	FallingRock* m_templateBullet;
	void CreateNewBullet();
public:
	FallingRockPool(int id, int iCapacity, int iInitPoolAmount, FallingRock* templateBullet);
	virtual ~FallingRockPool();
};

