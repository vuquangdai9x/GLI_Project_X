#pragma once

#include"StateBase.h"
#include"../Button.h"
#include"../UIComponent.h"
#include"../UIText.h"
class GS_PassLevelState : public StateBase
{
private:
	Sprite* background;
	std::vector<Button*> buttons;
	std::vector<UIComponent*> stars;
	UIText* level;
public:
	GS_PassLevelState();
	~GS_PassLevelState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
	void LoadScene(char* dataScene);
};