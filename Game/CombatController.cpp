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
	for (int i = 0; i < m_bulletStorages.size(); i++) {
		delete m_bulletStorages[i];
	}
}

void CombatController::Fire()
{
	int bulletAmount = m_bulletStorages[m_iCurrentBulletIndex]->GetCountBullet();
	if (bulletAmount <= 0) {
		printf("Out of ammo\n");
		return;
	}
	//printf("Player: %f,%f \t Target: %f,%f \n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_targetPos.x, m_targetPos.y);
	Vector2 direction;
	direction.x = m_targetPos.x - m_pPlayer->GetPosition().x;
	direction.y = m_targetPos.y - m_pPlayer->GetPosition().y;
	if (m_weapons[m_iCurrentWeaponIndex]->Fire(m_pPlayer, direction)) {
		// TODO: add opposite force to player
		m_weapons[m_iCurrentWeaponIndex]->GetOppositeForce();
	}
}

void CombatController::AddWeapon(Weapon* newWeapon)
{
	for (int i = 0; i < m_weapons.size(); i++) {
		if (m_weapons[i]->GetId() == newWeapon->GetId()) {
			printf("[msg] CombatController: Add existing weapon %d: %s\n", newWeapon->GetId(), newWeapon->GetName());
			return; // weapon existed
		}
	}

	bool isExistBulletType = false;
	int iBulletStorageIndex = 0;
	for (int i = 0; i < m_bulletStorages.size(); i++) {
		if (m_bulletStorages[i]->GetId() == newWeapon->GetbulletTypeId()) {
			isExistBulletType = true;
			iBulletStorageIndex = i;
			break;
		}
	}
	if (!isExistBulletType) {
		printf("[ERR] CombatController: There is no bullet pool for weapon %d: %s\n", newWeapon->GetId(), newWeapon->GetName());
	}
	else {
		printf("[msg] CombatController: Add new Weapon %d: %s\n", newWeapon->GetId(), newWeapon->GetName());
		m_weapons.push_back(newWeapon);
		newWeapon->BindBulletPool(m_bulletStorages[iBulletStorageIndex]);
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

	int iCurrentBulletTypeId = m_weapons[index]->GetbulletTypeId();
	for (int i = 0; i < m_bulletStorages.size(); i++) {
		if (m_bulletStorages[i]->GetId() == iCurrentBulletTypeId) {
			m_iCurrentBulletIndex = i;
			break;
		}
	}
}

void CombatController::AddBullet(int iBulletTypeId, int iAmount)
{
	for (int i = 0;i < m_bulletStorages.size(); i++) {
		if (m_bulletStorages[i]->GetId() == iBulletTypeId) {
			m_bulletStorages[i]->AddBullet(iAmount);
			break;
		}
	}
}

void CombatController::AddBulletPool(BulletPool* pool)
{
	for (int i = 0;i < m_bulletStorages.size(); i++) {
		if (pool->GetId() == m_bulletStorages[i]->GetId()) {
			return;
		}
	}
	m_bulletStorages.push_back(pool);
}

Vector2 CombatController::GetTargetPosition()
{
	return m_targetPos;
}