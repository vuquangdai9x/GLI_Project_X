#include "AutoGun.h"
#include <stdlib.h> 
#include <time.h>
#include "SoundManager.h"

AutoGun::AutoGun(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce, float rechargeTime, float shortRechargeTime, float randomAngle, float spreadRandomAngle, int iFireAtOnce, int iFireAmount)
	:Weapon(id, name, iWeaponTexId, iTargetTexId, iBulletTypeId, oppositeForce),
	m_rechargeTime(rechargeTime),
	m_shortRechargeTime(shortRechargeTime),
	m_randomAngle(randomAngle),
	m_spreadRandomAngle(spreadRandomAngle),
	m_iFireAtOnce(iFireAtOnce),
	m_iFireAmount(iFireAmount)
{
	m_timeCounter = m_shortTimeCounter = -1;
	m_iShotCounter = m_iFireAmount;

	srand(time(NULL));
}

int AutoGun::Fire(int index, Sprite* shooter, Vector2 direction)
{
	int iFireAmount = 0;
	if (m_timeCounter <= 0) {
		m_timeCounter = m_rechargeTime;
		m_shortTimeCounter = -1;
		m_iShotCounter = m_iFireAmount;
	}

	if (m_iShotCounter > 0 && m_shortTimeCounter <= 0) {
		m_iShotCounter--;
		m_shortTimeCounter = m_shortRechargeTime;

		Vector2 startPosition;
		startPosition.x = shooter->GetPosition().x;
		startPosition.y = shooter->GetPosition().y;

		float maxRandomAngle;
		if (m_iFireAmount > 1)
			maxRandomAngle = m_spreadRandomAngle - (m_spreadRandomAngle - m_randomAngle) / (m_iFireAmount - 1) * m_iShotCounter;
		else
			maxRandomAngle = m_randomAngle;
		Vector2 bulletDirecion;
		float randomAngle;

		for (int i = 0; i < m_iFireAtOnce; i++) {
			Bullet* bullet = m_bulletPool->GetBullet();
			if (bullet != NULL) {
				Singleton<SoundManager>::GetInstance()->Fire(index);
				iFireAmount++;

				randomAngle = maxRandomAngle * (((float)rand() / (float)RAND_MAX + i) / m_iFireAtOnce - 0.5);

				bulletDirecion.x = direction.x * cosf(randomAngle) - direction.y * sinf(randomAngle);
				bulletDirecion.y = direction.x * sinf(randomAngle) + direction.y * cosf(randomAngle);

				bullet->Fire(index, shooter, startPosition, bulletDirecion);
			}
		}
	}
	return iFireAmount;
}

void AutoGun::UpdateGunStatus(float deltaTime)
{
	if (m_timeCounter > 0) {
		m_timeCounter -= deltaTime;
	}
	if (m_shortTimeCounter > 0) {
		m_shortTimeCounter -= deltaTime;
	}
}
