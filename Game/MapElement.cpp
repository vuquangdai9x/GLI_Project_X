#include "stdafx.h"
#include "MapElement.h"

MapElement::MapElement(int id, char* name): m_id(id)
{
	if (strlen(name) >= 50) {
		strncpy(m_name, name, 49);
	}
	else {
		strcpy(m_name, name);
	}
}
