#pragma once
#include "Bullet.h"
#include <vector>
class BulletPool {
protected:
	int m_id;
	std::vector<Bullet*> m_listBullet;
	int m_iCapacity;
	int m_iCount;
protected:
	virtual void CreateNewBullet() = 0; // call when not enough bullet available in pool
	Bullet* GetBulletFromPool();
public:
	BulletPool(int id, int iCapacity);
	virtual ~BulletPool();
	Bullet* GetBullet();
	void AddBullet(int iAmount);
	int GetId();
	int GetCountBullet();
	int GetCapacity();
};