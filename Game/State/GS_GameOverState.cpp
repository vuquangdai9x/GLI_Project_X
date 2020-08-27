#include "../stdafx.h"
#include "GS_GameOverState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"

GS_GameOverState::GS_GameOverState()
{
	printf("GS_PauseState");
	char sceneFile[50] = "Datas/scene2d-gameover.txt";
	char resourcesFile[60] = "Datas/resources2d.txt";

	//Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);

	button = Singleton<SceneManager2D>::GetInstance()->LoadGameOverScene(sceneFile);
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->UpdateMember();
	}
}

GS_GameOverState::~GS_GameOverState()
{
}
void BackToMenu1()
{
	Singleton<GameStateManager>::GetInstance()->Pop();
}
void Reload1()
{
	Singleton<GameStateManager>::GetInstance()->Pop();
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
bool GS_GameOverState::Create()
{
    return false;
}

bool GS_GameOverState::Release()
{
    return false;
}

void GS_GameOverState::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render(GAMEOVER_OBJECT);
}

void GS_GameOverState::Update(float deltaTime)
{
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->Update(deltaTime);
		if (i == 0) {
			button[i]->OnClick(BackToMenu1);
		}
		else if (i == 1) {
			button[i]->OnClick(Reload1);
		}
	}
	Singleton<InputManager>::GetInstance()->fixButton();
}

void GS_GameOverState::KeyPress()
{
}
