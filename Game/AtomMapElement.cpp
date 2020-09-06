#include "AtomMapElement.h"
#include "MapElementManager.h"

void AtomMapElement::Generate(float difficulty, float bottom, float top, float left, float right)
{
	Vector3 pos((left + right) / 2, (top + bottom) / 2, 15);
	Singleton<MapElementManager>::GetInstance()->AddObjectToList(m_iObjTypeId, pos, Vector2(1, 1), 0, 0x000000, 1.0);
}
