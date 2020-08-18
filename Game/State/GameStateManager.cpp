#include "../stdafx.h"
#include "GameStateManager.h"
#include"GS_WelcomScreen.h"
#include"GS_MainMenu.h"
#include"GS_PlayState.h"
#include"../Singleton.h"
#include"../SceneManager2D.h"

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
	Singleton<SceneManager2D>::GetInstance()->GetListObject().clear();
	this->states.pop();
}

void GameStateManager::render()
{
	if (!this->states.empty()) {
		this -> states.top()->Render();
	}
}

void GameStateManager::update(float deltaTime)
{
	if (!this->states.empty()) {
		this->states.top()->Update(deltaTime);
	}
}

void GameStateManager::KeyPress()
{
	if (!this->states.empty()) {
		this->states.top()->KeyPress();
	}
}
