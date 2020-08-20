#pragma once
#include "Weapon.h"
class AutoGun : public Weapon {
protected:
	float m_rechargeTime, m_shortRechargeTime;
	float m_randomAngle, m_spreadRandomAngle;
	int m_iFireAtOnce; // number of bullet fire at one shot
	int m_iFireAmount; // number of shot before recharge
	float m_timeCounter, m_shortTimeCounter;
	int m_iShotCounter;
public:
	AutoGun(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce, float rechargeTime, float shortRechargeTime, float randomAngle, float spreadRandomAngle, int iFireAtOnce, int iFireAmount);
	int Fire(Player* player, Vector2 direction);
	void UpdateGunStatus(float deltaTime);
};