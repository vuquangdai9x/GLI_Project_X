#include "Weapon.h"
#include "ResourcesManager2D.h"
#include "Singleton.h"

Weapon::Weapon(int id, char* name, int iWeaponTexId, int iTargetTexId, BulletType bulletType, float oppositeForce):
m_id(id),m_name(name),m_bulletType(bulletType),m_oppositeForce(oppositeForce)
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

BulletType Weapon::GetbulletType()
{
	return m_bulletType;
}
