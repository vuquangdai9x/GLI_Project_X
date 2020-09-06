#pragma once
#include "MapElement.h"
#include "GameObjectPool.h"
#include <vector>
#include <map>
#include "Singleton.h"
class MapElementManager {
private:
	int m_iCountPoolId;
	std::vector<GameObjectPool*> m_listPool;
	std::vector<MapElement*> m_listMapElement;
	std::vector<std::vector<int>*> m_listElementIndexByTheme;
	//std::vector<std::vector<MapElement*>> m_listElementByTheme;

	std::vector<Sprite*>* m_currentList;
public:
	MapElementManager();
	~MapElementManager();
	void AddObjectType(Sprite* templateObj);
	void AddMapElement(MapElement* element);

	void AddObjectToList(int iObjTypeId, Vector3 position, Vector2 scale, float rotation, unsigned int uiHexColor, float alpha);
	void AddObjectToList(int iObjTypeId, Vector3 position, Vector2 scale, float rotation, Vector4 color);
	int GetRandomElementByObjectType(int iObjectTypeId);
	void SetUseList(std::vector<Sprite*>* list);
};