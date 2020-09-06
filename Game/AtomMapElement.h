#pragma once
#include "MapElement.h"
class AtomMapElement :public MapElement {
protected:
	int m_iObjTypeId;
public:
	AtomMapElement(int id, char* name, int iObjTypeId);
	virtual void Generate(float difficulty, float bottom, float top, float left, float right);
};