#pragma once
#include"StateBase.h"
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
	const static int QUIT = 3;
};

