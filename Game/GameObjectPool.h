#pragma once
#include "Sprite.h"
#include <vector>
class GameObjectPool {
private:
	int m_id;
	int m_iObjectIdCount;
	Sprite* m_template;
	std::vector<Sprite*> m_listObject;
public:
	GameObjectPool(int id, Sprite* templateObj);
	virtual ~GameObjectPool();
	Sprite* GetGameObject();
	int GetId();
};