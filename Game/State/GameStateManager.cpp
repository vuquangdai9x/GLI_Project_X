#include "GameStateManager.h"

GameStateManager::GameStateManager()
{
	this->states.push(new GS_WelcomScreen());
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::Push(int state)
{
	if (state == WELCOM) {
		this->states.push(new GS_WelcomScreen());
	}
	else if (state == MENU) {
		this->states.push(new GS_MainMenu());
	}
	else if (state == PLAY) {
		this->states.push(new GS_PlayState());
	}
	
}

void GameStateManager::Pop()
{
	this->states.pop();
}

void GameStateManager::render()
{
	if (!this->states.empty()) {
		this -> states.top()->Render();
	}
}

void GameStateManager::update()
{
	if (!this->states.empty()) {
		this->states.top()->Update();
	}
}

void GameStateManager::KeyPress(unsigned char key, bool bIsPressed)
{
	if (!this->states.empty()) {
		this->states.top()->KeyPress(key, bIsPressed);
	}
}
