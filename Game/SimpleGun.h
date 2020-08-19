#pragma once
#include "Weapon.h"
class SimpleGun : public Weapon {
protected:
	float m_rechargeTime, m_shortRechargeTime;
	float m_randomAngle, m_spreadRandomAngle;
	int m_iFireAtOnce; // number of bullet fire at one shot
	int m_iFireAmount; // number of shot before recharge
	float m_timeCounter, m_shortTimeCounter;
	int m_iShotCounter;
public:
	SimpleGun(int id, char* name, int iWeaponTexId, int iTargetTexId, BulletType bulletType, float oppositeForce, float rechargeTime, float shortRechargeTime, float randomAngle, float spreadRandomAngle, int iFireAtOnce, int iFireAmount);
	int Fire(Player* player, Vector2 direction, int iBulletAmountCanUse);
	void UpdateGunStatus(float deltaTime);
};