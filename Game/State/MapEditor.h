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
	Sprite* checkInside(float x, float y);
private:
	std::vector<UnitButton*> button;
	int m_mouseDragCheck=0;
	std::vector<Sprite*> m_Obstacle;
	UnitInfor *m_curent ;
	Sprite* m_currentSprite ;
};


