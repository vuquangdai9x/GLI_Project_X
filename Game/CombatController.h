#pragma once
#include "Weapon.h"
#include <vector>
#include "Player.h"
#include "BulletPool.h"

class CombatController {
private:
	Player* m_pPlayer;
	std::vector<Weapon*> m_weapons;
	int m_iCurrentWeaponIndex;
	int m_iCurrentBulletIndex;
	std::vector<BulletPool*> m_bulletStorages;
	Vector2 m_targetPos;

	bool m_isFiring;
private:
	void Fire();
public:
	CombatController(Player* player);
	~CombatController();
	void AddWeapon(Weapon* newWeapon);
	void Update(float deltaTime);
	void ChangeWeapon(int index);
	void AddBullet(int iBulletTypeId, int iAmount);
	void AddBulletPool(BulletPool* pool);
	Vector2 GetTargetPosition();
};