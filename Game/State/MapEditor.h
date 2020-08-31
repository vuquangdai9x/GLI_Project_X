#pragma once
#include "StateBase.h"
#include"../Button.h"
#include"../UnitButton.h"
#include "../Sprite.h"
#include<vector>
class MapEditor : public StateBase
{
public:
	MapEditor();
	~MapEditor();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);
	void addObject();
	void KeyPress();
	void deleteSprite(Sprite* pointer);
	Sprite* checkInside(float x, float y);
	void pushSprite(Sprite* pointer);
	Sprite* cloneSprite(Sprite* source);
private:
	std::vector<UnitButton*> button;
	int m_mouseDragCheck=0;
	int m_norColor = 0xffffff;
	int m_chooseColor = 0xffafff;

	std::vector<Sprite*> m_Obstacle,m_suicudeBug;
	std::vector<Sprite*> m_activeList;
	std::vector<float> m_distance_x, m_distance_y;
	UnitInfor *m_curent ;
	Sprite* m_currentSprite ;
};


