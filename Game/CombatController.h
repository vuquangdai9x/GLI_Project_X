#pragma once
#include "Weapon.h"
#include "BulletPackage.h"
#include <vector>
#include "Player.h"
class CombatController {
private:
	Player* m_pPlayer;
	std::vector<Weapon*> m_weapons;
	int m_iCurrentWeaponIndex;
	int m_iCurrentBulletIndex;
	std::vector<BulletPackage*> m_bulletPackages;
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
	void AddBullet(BulletPackage* pack);
	void AddBullet(BulletType bulletType, int iAmount);
	Vector2 GetTargetPosition();
};