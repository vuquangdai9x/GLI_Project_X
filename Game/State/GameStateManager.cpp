#include "../stdafx.h"
#include "GameStateManager.h"
#include"GS_WelcomScreen.h"
#include"GS_MainMenu.h"
#include"GS_PlayState.h"
#include"GS_PauseState.h"
#include"../Singleton.h"
#include"../SceneManager2D.h"

GameStateManager::GameStateManager()
{
	this->states.push(new GS_WelcomScreen());
	//this->states.push(new GS_PlayState());
	pauseState = new GS_PauseState();
	this->paused = false;
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
	if (paused) {
		pauseState->Render();
	}
	else {
		if (!this->states.empty()) {
			this->states.top()->Render();
		}
	}	
}

void GameStateManager::update(float deltaTime)
{
	if (paused) {
		pauseState->Update(deltaTime);
	}
	else {
		if (!this->states.empty()) {
			this->states.top()->Update(deltaTime);
		}
	}	
}

void GameStateManager::KeyPress()
{
	if (paused) {
		pauseState->KeyPress();
	}
	else {
		if (!this->states.empty()) {
			this->states.top()->KeyPress();
		}
	}
}
