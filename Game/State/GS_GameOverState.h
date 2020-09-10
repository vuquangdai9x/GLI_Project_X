#pragma once

#include"StateBase.h"
#include"../Button.h"
#include"../Sprite.h"
#include"../UIText.h"
class GS_GameOverState : public StateBase
{
private:
	std::vector<Button*> buttons;
	Sprite* background;
	UIText *gameover;
	UIText *score;
public:
	GS_GameOverState() ;
	~GS_GameOverState() ;
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
	void loadGameOver(char* dataSceneFile);
};
