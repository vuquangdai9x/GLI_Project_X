#include "../stdafx.h"
#include "GS_PauseState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include"../WorldManager.h"
GS_PauseState::GS_PauseState()
{
	printf("GS_PauseState");
	char sceneFile[50] = "Datas/scene2d-pause.txt";
	char resourcesFile[60] = "Datas/resources2d.txt";

	Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);

	button = Singleton<SceneManager2D>::GetInstance()->LoadPauseScene(sceneFile);
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->UpdateMember();
	}
	this->misson = NULL;
}

GS_PauseState::~GS_PauseState()
{
	for (int i = 0; i < button.size(); i++) {
		delete button[i];
	}
	std::vector<Button*>().swap(button);
	delete misson;
}

void BackToMenu()
{
	Singleton<GameStateManager>::GetInstance()->setPaused(false);
	Singleton<GameStateManager>::GetInstance()->Pop();
}
void Resume()
{
	Singleton<GameStateManager>::GetInstance()->setPaused(false);
}
void Reload()
{
	Singleton<GameStateManager>::GetInstance()->setPaused(false);
	Singleton<GameStateManager>::GetInstance()->Pop();
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
bool GS_PauseState::Create()
{
	return false;
}

bool GS_PauseState::Release()
{
	return false;
}

void GS_PauseState::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render();
	Singleton<SceneManager2D>::GetInstance()->Render(PAUSE_OBJECT);
	if (this->misson != NULL) {
		this->misson->Render();
	}
}

void GS_PauseState::Update(float deltaTime)
{
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->Update(deltaTime);
		if (i == 0) {
			button[i]->OnClick(BackToMenu);
		}
		else if (i == 1) {
			button[i]->OnClick(Resume);
		}
		else if (i == 2) {
			button[i]->OnClick(Reload);
		}
	}
	Singleton<InputManager>::GetInstance()->fixButton();
}

void GS_PauseState::KeyPress()
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::ESCAPE)) {
		Singleton<GameStateManager>::GetInstance()->setPaused(false);
	}
}


