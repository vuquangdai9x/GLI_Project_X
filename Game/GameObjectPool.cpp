#include "GameObjectPool.h"

GameObjectPool::GameObjectPool(int id, Sprite* templateObj) : m_id(id), m_template(templateObj)
{
	m_iObjectIdCount = templateObj->GetId();
	Sprite* newObj;
	for (int i = 0; i < 10; i++) {
		m_iObjectIdCount++;
		newObj = m_template->CreateClone(m_iObjectIdCount);
		m_listObject.push_back(newObj);
	}
}

GameObjectPool::~GameObjectPool()
{
	for (int i = 0;i < m_listObject.size();i++) {
		delete m_listObject[i];
	}
	m_listObject.clear();
	if (m_template != NULL) delete m_template;
}

Sprite* GameObjectPool::GetGameObject()
{
	for (int i = 0;i < m_listObject.size();i++) {
		if (!m_listObject[i]->CheckIsActiveSprite()) {
			return m_listObject[i];
		}
	}
	m_iObjectIdCount++;
	Sprite* newObj = m_template->CreateClone(m_iObjectIdCount);
	m_listObject.push_back(newObj);
	return newObj;
}

int GameObjectPool::GetId()
{
	return m_id;
}
