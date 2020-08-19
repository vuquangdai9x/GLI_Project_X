#include "CombatController.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "Singleton.h"
#include "SceneManager2D.h"

CombatController::CombatController(Player* player): m_pPlayer(player)
{
	m_iCurrentBulletIndex = 0;
	m_iCurrentWeaponIndex = 0;
	m_isFiring = false;
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
	if (bulletAmount <= 0) {
		return;
	}
	//printf("Player: %f,%f \t Target: %f,%f \n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_targetPos.x, m_targetPos.y);
	Vector2 direction;
	direction.x = m_targetPos.x - m_pPlayer->GetPosition().x;
	// TODO: y is invert when convert from 2d to 3d. When it is fixed, change the code below
	direction.y = m_targetPos.y + m_pPlayer->GetPosition().y;
	int iFiredAmount = m_weapons[m_iCurrentWeaponIndex]->Fire(m_pPlayer, direction, bulletAmount);
	if (iFiredAmount > 0) {
		// TODO: add opposite force to player
		m_weapons[m_iCurrentWeaponIndex]->GetOppositeForce();

		m_bulletPackages[m_iCurrentBulletIndex]->m_iAmount -= iFiredAmount;
	}
}

void CombatController::AddWeapon(Weapon* newWeapon)
{
	for (int i = 0; i < m_weapons.size(); i++) {
		if (m_weapons[i]->GetId() == newWeapon->GetId()) {
			return; // weapon existed
		}
	}
	printf("[msg] CombatController: Add new Weapon: %s\n",newWeapon->GetName());
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
	// update target position
	int iMouseScreenPosX, iMouseScreenPosY;
	Singleton<InputManager>::GetInstance()->getXY(iMouseScreenPosX, iMouseScreenPosY);
	Vector3 targetPos = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMouseScreenPosX, iMouseScreenPosY);
	m_targetPos.x = targetPos.x;
	m_targetPos.y = targetPos.y;
	// TODO: update target UI

	if (m_weapons.size() == 0) return;

	// check switch weapon. Check num key 1-9
	for (int i = 0; i < m_weapons.size() && i < 9; i++) {
		if (Singleton<InputManager>::GetInstance()->GetBit((InputManager::Key)(i+InputManager::Key::N1)) == 1) {
			ChangeWeapon(i);
			break;
		}
	}

	for (int i = 0; i < m_weapons.size(); i++) {
		m_weapons[i]->UpdateGunStatus(deltaTime);
	}

	if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) {
		m_isFiring = true;
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_RELEASE) {
		m_isFiring = false;
	}
	if (m_isFiring) {
		Fire();
	}
}

void CombatController::ChangeWeapon(int index)
{
	if (index >= m_weapons.size()) return;
	if (index == m_iCurrentWeaponIndex) return;
	printf("[msg] CombatController: Change weapon %d: %s\n", index, m_weapons[index]->GetName());

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
	printf("[msg] CombatController: Add %d bullet\n", pack->m_iAmount);
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

void CombatController::AddBullet(BulletType bulletType, int iAmount)
{
	printf("[msg] CombatController: Add %d bullet\n", iAmount);
	bool isExistBulletType = false;
	for (int i = 0; i < m_bulletPackages.size(); i++) {
		if (m_bulletPackages[i]->m_bulletType == bulletType) {
			m_bulletPackages[i]->m_iAmount += iAmount;
			isExistBulletType = true;
			break;
		}
	}
	if (!isExistBulletType) {
		m_bulletPackages.push_back(new BulletPackage(bulletType, iAmount));
	}
}

Vector2 CombatController::GetTargetPosition()
{
	return m_targetPos;
}
