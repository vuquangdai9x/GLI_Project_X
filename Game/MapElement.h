#pragma once
#pragma warning(disable: 4996)
class MapElement {
protected:
	int m_id;
	char m_name[50];
public:
	MapElement(int id, char* name);
	virtual void Generate(float difficulty, float bottom, float top, float left, float right) = 0;
};