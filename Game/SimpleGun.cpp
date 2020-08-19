#include "SimpleGun.h"

SimpleGun::SimpleGun(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce, float rechargeTime, float shortRechargeTime, float randomAngle, float spreadRandomAngle, int iFireAtOnce, int iFireAmount)
	:Weapon(id, name, iWeaponTexId, iTargetTexId, iBulletTypeId, oppositeForce),
	m_rechargeTime(rechargeTime),
	m_shortRechargeTime(shortRechargeTime),
	m_randomAngle(randomAngle),
	m_spreadRandomAngle(spreadRandomAngle),
	m_iFireAtOnce(iFireAtOnce),
	m_iFireAmount(iFireAmount)
{
	m_timeCounter = -1;
	m_iShotCounter = m_iFireAmount;
}

int SimpleGun::Fire(Player* player, Vector2 direction)
{
	if (m_timeCounter <= 0) {
		int iFireAmount = 0;
		Vector2 startPosition;
		startPosition.x = player->GetPosition().x;
		startPosition.y = player->GetPosition().y;
		for (int i = 0; i < m_iFireAtOnce; i++) {
			Bullet* bullet = m_bulletPool->GetBullet();
			if (bullet != NULL) {
				iFireAmount++;
				bullet->Fire(player, startPosition, direction);
			}
		}
		printf("Fire %d bullet by gun %s: direction=%f,%f\n", iFireAmount, m_name, direction.x, direction.y);
		/*m_iShotCounter--;
		if (m_iShotCounter <= 0) {
			m_timeCounter = m_rechargeTime;
			m_iShotCounter = m_iFireAmount;
		}
		else {
			m_timeCounter = m_shortRechargeTime;
		}*/
		m_timeCounter = m_rechargeTime;
		return iFireAmount;
	}
	else {
		return 0;
	}
}

void SimpleGun::UpdateGunStatus(float deltaTime)
{
	if (m_timeCounter > 0) {
		m_timeCounter -= deltaTime;
	}
}
