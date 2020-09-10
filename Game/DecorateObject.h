#pragma once
#include "Sprite.h"
#include "ItemBody.h"

class DecorateObject :public Sprite
{
private:
	int m_id;
	int m_iDamagedEffectId;
	bool m_isDestroyed;
	bool m_isDeleteOnDestroyed;
	ItemBody* m_physicsBody;
	Vector2 m_colliderRatio;
	Vector2 m_pivot;
public:
	DecorateObject(int id);
	~DecorateObject();
	DecorateObject* CreateClone(int id);
	void CreatePhysicsBody();
	void InitInfo(Vector2 pivot, bool isDeleteOnDestroyed, int iDamagedEffectId, Vector2 colliderRatio);
	void Update(float deltaTime);
	void SetPosition(Vector3 position);
};

