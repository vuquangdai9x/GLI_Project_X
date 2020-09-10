#include "../stdafx.h"
#include "GameStateManager.h"
#include"GS_WelcomScreen.h"
#include"GS_MainMenu.h"
#include"GS_PlayState.h"
#include"GS_PauseState.h"
#include "MapEditor.h"
#include"GS_GameOverState.h"
#include"GS_TutorialState.h"
#include"GS_PassLevelState.h"
#include"GS_SelectLevel.h"
#include"../Singleton.h"
#include"../SceneManager2D.h"
#include"../SoundManager.h"
#include "../WorldManager.h"
#include "../PostProcessingManager.h"

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
		
		Singleton<SoundManager>::GetInstance()->Click();
		this->states.push(new GS_PlayState());
	}
	else if (state == GAMEOVER) {
		this->states.push(new GS_GameOverState());
	}
	else if (state == MAP) {
		this->states.push(new MapEditor());
	}
	else if (state == QUIT) {
		Singleton<SoundManager>::GetInstance()->Click();
		exit(1);
	}
	else if (state == TUTORIAL) {
		this->states.push(new GS_TutorialState());
	}
	else if (state == PASSLEVEL) {
		this->states.push(new GS_PassLevelState());
	}
	else if (state == SELECTLEVEL) {
		this->states.push(new GS_SelectLevel());
	}
}

void GameStateManager::Pop()
{
	StateBase *state = this->states.top();
	this->states.pop();
	if (dynamic_cast<GS_MainMenu*>(state)) {
		GS_MainMenu* del = dynamic_cast<GS_MainMenu*>(state);
		delete del;
	} 
	else if (dynamic_cast<GS_PlayState*>(state)) {
		GS_PlayState* del = dynamic_cast<GS_PlayState*>(state);
		ShowCursor(true);
		delete del;
	}
	else if (dynamic_cast<GS_PauseState*>(state)) {
		GS_PauseState* del = dynamic_cast<GS_PauseState*>(state);
		delete del;
	}
	else if (dynamic_cast<GS_PassLevelState*>(state)) {
		GS_PassLevelState* del = dynamic_cast<GS_PassLevelState*>(state);
		delete del;
	}
	else if (dynamic_cast<GS_SelectLevel*>(state)) {
		GS_SelectLevel* del = dynamic_cast<GS_SelectLevel*>(state);
		delete del;
	}
	else if (dynamic_cast<GS_GameOverState*>(state)) {
		GS_GameOverState* del = dynamic_cast<GS_GameOverState*>(state);
		delete del;
	}
	else if (dynamic_cast<GS_TutorialState*>(state)) {
		GS_TutorialState* del = dynamic_cast<GS_TutorialState*>(state);
		delete del;
	}

	Singleton<SceneManager2D>::GetInstance()->CleanUp();
	Singleton<WorldManager>::GetInstance()->CleanUp();
}

void GameStateManager::DeleteMap()
{

	MapEditor *state = dynamic_cast<MapEditor*> (this->states.top());
	this->states.pop();
	delete state;
}

void GameStateManager::render()
{
	Singleton<PostProcessingManager>::GetInstance()->Prepare();
	if (paused) {
		pauseState->Render();
	}
	else {
		if (!this->states.empty()) {
			this->states.top()->Render();
		}
	}	
	Singleton<PostProcessingManager>::GetInstance()->Render();
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
