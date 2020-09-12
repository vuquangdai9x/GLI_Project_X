#pragma once
#pragma warning(disable: 4996)
#include "Singleton.h"
#include "DecorateObject.h"
#include <vector>

class DecorateObjectManager
{
private:
	int m_iObjIdCount;
	std::vector<DecorateObject*> m_listTemplateObject;

	bool LoadAnimation(FILE* fIn, DecorateObject* sprite);
public:
	DecorateObjectManager();
	~DecorateObjectManager();
	void Load(char* decorateObjFilePath);
	void AddTemplateObject(DecorateObject* obj);
	DecorateObject* CreateObject(int iObjId, Vector2 position);
	DecorateObject* CreateRandomObject(Vector2 position);
	DecorateObject* CreateObjectByIndex(int iObjIndex, Vector2 position);
};

