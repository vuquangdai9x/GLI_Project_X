#pragma once

#include"StateBase.h"
#include"../Button.h"
#include"../UIComponent.h"
#include"../UIText.h"
class GS_SelectLevel : public StateBase
{
private:
	Sprite* background;
	std::vector<Button*> buttons;
	std::vector<UIText*> levels;
	std::vector<UIComponent*> lock;
	UIText* level;
	int numberOfLevel;

	Button* bt;
	UIComponent* star;
public:
	GS_SelectLevel();
	~GS_SelectLevel();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
	void LoadScene(char* dataScene);
};