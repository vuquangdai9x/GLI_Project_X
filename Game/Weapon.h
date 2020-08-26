#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "Player.h"
#include "BulletPool.h"

class Weapon {
protected:
	int m_id;
	char* m_name;
	Texture* m_weaponTexture;
	Texture* m_targetTexture;
	int m_iBulletTypeId;
	float m_oppositeForce;
	BulletPool* m_bulletPool;
public:
	Weapon(int id, char* name, int iWeaponTexId, int iTargetTexId, int iBulletTypeId, float oppositeForce);
	virtual ~Weapon();
	virtual int Fire(Sprite* shooter, Vector2 direction) = 0;
	virtual float GetOppositeForce();
	virtual void UpdateGunStatus(float deltaTime) = 0;
	void BindBulletPool(BulletPool* pool);
	int GetId();
	char* GetName();
	Texture* GetTargetTexture();
	Texture* GetWeaponTexture();
	int GetbulletTypeId();
};