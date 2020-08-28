#pragma once
#include"StateBase.h"
#include<stack>
#include "GS_PauseState.h"


class GameStateManager
{
private:
	std::stack<StateBase*> states;
	bool paused;
	GS_PauseState* pauseState;
public:
	GameStateManager();
	~GameStateManager();
	void Push(int state);
	void Pop();
	void render();
	void update(float deltaTime);
	void KeyPress();
	void setPaused(bool p) { this->paused = p; }

	const static int WELCOM = 0;
	const static int MENU = 1;
	const static int PLAY = 2;
	const static int GAMEOVER = 3;
	const static int MAP = 4;
};

