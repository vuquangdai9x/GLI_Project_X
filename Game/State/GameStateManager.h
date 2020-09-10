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
	int map = 1;
public:
	GameStateManager();
	~GameStateManager();
	void Push(int state);
	void Pop();
	void DeleteMap();
	void render();
	void update(float deltaTime);
	void KeyPress();
	void setPaused(bool p) { 
		ShowCursor(p);

		this->paused = p; }
	StateBase *getTop() { return this->states.top(); }
	GS_PauseState* getPauseState() { return this->pauseState; }
	void setMap(int map) { this->map = map; }
	int getMap() { return this->map; }

	const static int WELCOM = 0;
	const static int MENU = 1;
	const static int PLAY = 2;
	const static int GAMEOVER = 3;
	const static int MAP = 4;
	const static int QUIT = 5;
	const static int TUTORIAL = 6;
	const static int PASSLEVEL = 7;
	const static int SELECTLEVEL = 8;
};

