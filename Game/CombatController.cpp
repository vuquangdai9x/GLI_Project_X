#include "CombatController.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include "SoundManager.h"

#include "EffectManager.h"

CombatController::CombatController(Player* player): m_pPlayer(player)
{
	m_iCurrentBulletIndex = -1;
	m_iCurrentWeaponIndex = -1;
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
		//printf("Out of ammo\n");
		return;
	}
	//printf("Player: %f,%f \t Target: %f,%f \n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_targetPos.x, m_targetPos.y);
	Vector2 direction;
	direction.x = m_targetPos.x - m_pPlayer->GetPosition().x;
	direction.y = m_targetPos.y - m_pPlayer->GetPosition().y;
	int iNumBulletFired = m_weapons[m_iCurrentWeaponIndex]->Fire(m_iCurrentWeaponIndex, m_pPlayer, direction);
	if (iNumBulletFired) {
		// TODO: add opposite force to player
		
		m_pPlayer->getHUDController()->UpdateBulletStatus(
			m_bulletStorages[m_iCurrentBulletIndex]->GetCountBullet(),
			m_bulletStorages[m_iCurrentBulletIndex]->GetCapacity()
		);

		// TODO: add fire effect
		float oppForce = -5 * m_weapons[m_iCurrentWeaponIndex]->GetOppositeForce();
		m_pPlayer->getBody()->body->ApplyLinearImpulseToCenter(b2Vec2(direction.x * oppForce, 0.5 * direction.y * oppForce),true);
		float fireAngle = (direction.y > 0 ? 1 : -1) * acosf(direction.x / direction.Length());
		Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(m_pPlayer->GetPosition(), 11000, Vector2(1,1), fireAngle);
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
	m_pPlayer->getHUDController()->UpdateTargetPosition(
		iMouseScreenPosX / (float)Globals::screenWidth,
		iMouseScreenPosY / (float)Globals::screenHeight
	);

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
	Singleton<SoundManager>::GetInstance()->Player(SoundManager::P_CHANGE);
	printf("[msg] CombatController: Change weapon %d: %s\n", index, m_weapons[index]->GetName());
	m_iCurrentWeaponIndex = index;

	int iCurrentBulletTypeId = m_weapons[index]->GetbulletTypeId();

	m_pPlayer->getHUDController()->ChangeWeaponIcon(m_weapons[m_iCurrentWeaponIndex]->GetWeaponTexture());
	m_pPlayer->getHUDController()->ChangeTargetIcon(m_weapons[m_iCurrentWeaponIndex]->GetTargetTexture());

	for (int i = 0; i < m_bulletStorages.size(); i++) {
		if (m_bulletStorages[i]->GetId() == iCurrentBulletTypeId) {
			m_iCurrentBulletIndex = i;
			break;
		}
	}

	m_pPlayer->getHUDController()->UpdateBulletStatus(
		m_bulletStorages[m_iCurrentBulletIndex]->GetCountBullet(),
		m_bulletStorages[m_iCurrentBulletIndex]->GetCapacity()
	);
}

void CombatController::AddBullet(int iBulletTypeId, int iAmount)
{
	for (int i = 0;i < m_bulletStorages.size(); i++) {
		if (m_bulletStorages[i]->GetId() == iBulletTypeId) {
			m_bulletStorages[i]->AddBullet(iAmount);
			if (i == m_iCurrentBulletIndex) {
				m_pPlayer->getHUDController()->UpdateBulletStatus(
					m_bulletStorages[m_iCurrentBulletIndex]->GetCountBullet(),
					m_bulletStorages[m_iCurrentBulletIndex]->GetCapacity()
				);
			}
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
