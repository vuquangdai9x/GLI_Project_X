#pragma once
#include "Weapon.h"
class SimpleGun : public Weapon {
protected:
	float m_rechargeTime;
	float m_randomAngle;
	int m_iFireAtOnce; // number of bullet fire at one shot
	float m_timeCounter;
	bool m_firedPrevFrame, m_canFire;
public:
	SimpleGun(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce, float rechargeTime, float randomAngle, int iFireAtOnce);
	int Fire(Player* player, Vector2 direction);
	void UpdateGunStatus(float deltaTime);
};