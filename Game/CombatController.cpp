#include "CombatController.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "Singleton.h"
#include "SceneManager2D.h"

CombatController::CombatController(Player* player): m_pPlayer(player)
{
	m_iCurrentBulletIndex = 0;
	m_iCurrentWeaponIndex = 0;
}

CombatController::~CombatController()
{
	for (int i = 0; i < m_weapons.size(); i++) {
		delete m_weapons[i];
	}
	for (int i = 0; i < m_bulletPackages.size(); i++) {
		delete m_bulletPackages[i];
	}
}

void CombatController::Fire()
{
	int bulletAmount = m_bulletPackages[m_iCurrentBulletIndex]->m_iAmount;
	if (bulletAmount <= 0) return;
	Vector2 direction = m_targetPos;
	direction.x -= m_pPlayer->GetPosition().x;
	direction.y -= m_pPlayer->GetPosition().y;
	int iFiredAmount = m_weapons[m_iCurrentWeaponIndex]->Fire(m_pPlayer, direction, bulletAmount);
	if (iFiredAmount > 0) {
		// TODO: add opposite force to player
		m_weapons[m_iCurrentWeaponIndex]->GetOppositeForce();
	}
}

void CombatController::AddWeapon(Weapon* newWeapon)
{
	for (int i = 0; i < m_weapons.size(); i++) {
		if (m_weapons[i]->GetId() == newWeapon->GetId()) {
			return; // weapon existed
		}
	}

	m_weapons.push_back(newWeapon);
	bool isExistBulletType = false;
	for (int i = 0; i < m_bulletPackages.size(); i++) {
		if (m_bulletPackages[i]->m_bulletType == newWeapon->GetbulletType()) {
			isExistBulletType = true;
			break;
		}
	}
	if (!isExistBulletType) {
		m_bulletPackages.push_back(new BulletPackage(newWeapon->GetbulletType(), 0));
	}
}

void CombatController::Update(float deltaTime)
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N1) == 1) {
		ChangeWeapon(0);
	}
	else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N2) == 1) {
		ChangeWeapon(1);
	}
	else if(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N3) == 1) {
		ChangeWeapon(2);
	}
	else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N4) == 1) {
		ChangeWeapon(3);
	}
	if (m_weapons.size() == 0) return;
	// calculate target position
	int iMouseScreenPosX, iMouseScreenPosY;
	Singleton<InputManager>::GetInstance()->getXY(iMouseScreenPosX, iMouseScreenPosY);
	Vector3 targetPos = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMouseScreenPosX, iMouseScreenPosY);
	m_targetPos.x = targetPos.x;
	m_targetPos.y = targetPos.y;

	for (int i = 0; i < m_weapons.size(); i++) {
		m_weapons[i]->UpdateGunStatus(deltaTime);
	}

	if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK)
		Fire();
}

void CombatController::ChangeWeapon(int index)
{
	if (index >= m_weapons.size()) return;
	if (index == m_iCurrentBulletIndex) return;
	printf("Change weapon %d: %s\n", index+1, m_weapons[index]->GetName());

	m_iCurrentWeaponIndex = index;
	BulletType currBulletType = m_weapons[index]->GetbulletType();
	for (int i = 0; i < m_bulletPackages.size(); i++) {
		if (m_bulletPackages[i]->m_bulletType == currBulletType) {
			m_iCurrentBulletIndex = i;
			break;
		}
	}
}

void CombatController::AddBullet(BulletPackage* pack)
{
	bool isExistBulletType = false;
	for (int i = 0; i < m_bulletPackages.size(); i++) {
		if (m_bulletPackages[i]->m_bulletType == pack->m_bulletType) {
			m_bulletPackages[i]->m_iAmount += pack->m_iAmount;
			isExistBulletType = true;
			break;
		}
	}
	if (!isExistBulletType) {
		m_bulletPackages.push_back(new BulletPackage(pack->m_bulletType, pack->m_iAmount));
	}
}

Vector2 CombatController::GetTargetPosition()
{
	return m_targetPos;
}
