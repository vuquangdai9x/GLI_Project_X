#pragma once
#include"StateBase.h"
#include"GS_WelcomScreen.h"
#include"GS_MainMenu.h"
#include"GS_PlayState.h"
#include<stack>


class GameStateManager
{
private:
	std::stack<StateBase*> states;
public:
	GameStateManager();
	~GameStateManager();
	void Push(int state);
	void Pop();
	void render();
	void update(float deltaTime);
	void KeyPress();

	const static int WELCOM = 0;
	const static int MENU = 1;
	const static int PLAY = 2;
};

