#include "MapElementManager.h"

MapElementManager::MapElementManager() : m_iCountPoolId(0)
{
}

MapElementManager::~MapElementManager()
{
	for (int i = 0;i < m_listPool.size();i++) {
		delete m_listPool[i];
	}
	m_listPool.clear();
	for (int i = 0;i < m_listMapElement.size();i++) {
		delete m_listMapElement[i];
	}
	m_listMapElement.clear();
	for (int i = 0;i < m_listElementIndexByTheme.size();i++) {
		delete m_listElementIndexByTheme[i];
	}
	m_listElementIndexByTheme.clear();
}

void MapElementManager::AddObjectType(Sprite* templateObj)
{
	m_listPool.push_back(new GameObjectPool(m_iCountPoolId, templateObj));
	m_listElementIndexByTheme.push_back(new std::vector<int>);
	m_iCountPoolId++;
}

void MapElementManager::AddMapElement(MapElement* element)
{
}

void MapElementManager::AddObjectToList(int iObjTypeId, Vector3 position, Vector2 scale, float rotation, unsigned int uiHexColor, float alpha)
{
	if (m_currentList == NULL) return;
	Sprite * obj = m_listPool[iObjTypeId]->GetGameObject();
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRotation(rotation);
	obj->SetColor(uiHexColor, alpha);
	m_currentList->push_back(obj);
}

void MapElementManager::AddObjectToList(int iObjTypeId, Vector3 position, Vector2 scale, float rotation, Vector4 color)
{
	if (m_currentList == NULL) return;
	Sprite* obj = m_listPool[iObjTypeId]->GetGameObject();
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRotation(rotation);
	obj->SetColor(color);
	m_currentList->push_back(obj);
}

int MapElementManager::GetRandomElementByObjectType(int iObjectTypeId)
{
	int size = m_listElementIndexByTheme[iObjectTypeId]->size();
	return (*m_listElementIndexByTheme[iObjectTypeId])[rand() % size];
}

void MapElementManager::SetUseList(std::vector<Sprite*>* list) {
	m_currentList = list;
}
