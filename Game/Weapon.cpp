#include "Weapon.h"
#include "ResourcesManager2D.h"
#include "Singleton.h"

Weapon::Weapon(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce)
	: m_id(id), m_name(name), m_iBulletTypeId(iBulletTypeId), m_oppositeForce(oppositeForce), m_bulletPool(NULL)
{
	m_weaponTexture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iWeaponTexId);
	m_targetTexture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iTargetTexId);
}

float Weapon::GetOppositeForce()
{
	return m_oppositeForce;
}

void Weapon::BindBulletPool(BulletPool* pool)
{
	m_bulletPool = pool;
}

int Weapon::GetId()
{
	return m_id;
}

char* Weapon::GetName()
{
	return m_name;
}

Texture* Weapon::GetTargetTexture()
{
	return m_targetTexture;
}

Texture* Weapon::GetWeaponTexture()
{
	return m_weaponTexture;
}

int Weapon::GetbulletTypeId()
{
	return m_iBulletTypeId;
}
