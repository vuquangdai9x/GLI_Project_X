#pragma once
#include "BulletPool.h"
#include "GunBullet.h"

class GunBulletPool : public BulletPool {
public:
	GunBulletPool(int id, int iCapacity, GunBullet& templateBullet);
};