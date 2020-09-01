#pragma once

#include"StateBase.h"
#include"../Sprite.h"
#include"../UIText.h"
#include"../UIComponent.h"
#include<vector>

class GS_TutorialState : public StateBase
{
private:
	Sprite* background;
	std::vector<UIComponent*> data;
	UIText* text;

	int current;
public:
	GS_TutorialState();
	~GS_TutorialState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};