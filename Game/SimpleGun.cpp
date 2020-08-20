#include "SimpleGun.h"
#include <stdlib.h> 
#include <time.h>

SimpleGun::SimpleGun(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce, float rechargeTime, float randomAngle, int iFireAtOnce)
	:Weapon(id, name, iWeaponTexId, iTargetTexId, iBulletTypeId, oppositeForce),
	m_rechargeTime(rechargeTime),
	m_randomAngle(randomAngle),
	m_iFireAtOnce(iFireAtOnce)
{
	m_timeCounter = -1;
	m_firedPrevFrame = false;
	m_canFire = true;
	srand(time(NULL));
}

int SimpleGun::Fire(Player* player, Vector2 direction)
{
	int iFireAmount = 0;
	if (m_timeCounter <= 0 && m_canFire) {
		m_timeCounter = m_rechargeTime;

		Vector2 startPosition;
		startPosition.x = player->GetPosition().x;
		startPosition.y = player->GetPosition().y;

		Vector2 bulletDirecion;
		float randomAngle;

		for (int i = 0; i < m_iFireAtOnce; i++) {
			Bullet* bullet = m_bulletPool->GetBullet();
			if (bullet != NULL) {
				iFireAmount++;
				randomAngle = m_randomAngle * (((float)rand() / (float)RAND_MAX + i) / m_iFireAtOnce - 0.5);

				bulletDirecion.x = direction.x * cosf(randomAngle) - direction.y * sinf(randomAngle);
				bulletDirecion.y = direction.x * sinf(randomAngle) + direction.y * cosf(randomAngle);

				bullet->Fire(player, startPosition, bulletDirecion);
			}
		}
	}
	m_firedPrevFrame = true;
	return iFireAmount;
}

void SimpleGun::UpdateGunStatus(float deltaTime)
{
	if (m_timeCounter > 0) {
		m_timeCounter -= deltaTime;
	}
	if (m_firedPrevFrame) {
		m_canFire = false;
		m_firedPrevFrame = false;
	}
	else {
		m_canFire = true;
	}
}
