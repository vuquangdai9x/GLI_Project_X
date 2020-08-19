#pragma once
#include "BulletPool.h"
#include "CanonBullet.h"

class CannonBulletPool : public BulletPool {
public:
	CannonBulletPool(int id, int iCapacity, CanonBullet& templateBullet);
};