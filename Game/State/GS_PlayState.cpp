#include "../stdafx.h"
#include "GS_PlayState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../GunBullet.h"
#include "../FloatingFish.h"

GS_PlayState::GS_PlayState()
{
    printf("GS_PlayState\n");
	// 2D
	char sceneFile[50] = "Datas/scene2d.txt";
	char resourcesFile[50] = "Datas/resources2d.txt";

	//Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);

	if (!Singleton<SceneManager2D>::GetInstance()->LoadScene(sceneFile)) {
		printf("[ERR] Entry point: Failed to init scene");
		return ;
	}
}

GS_PlayState::~GS_PlayState()
{
}

bool GS_PlayState::Create()
{
    return false;
}

bool GS_PlayState::Release()
{
    return false;
}
void GS_PlayState::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render();
}
void GS_PlayState::Update(float deltaTime)
{
	Singleton<SceneManager2D>::GetInstance()->Update(deltaTime);
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Q), deltaTime);
	camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Z), deltaTime);

	// NOTE: for debug purpose. Player/HotAirBalloon must inherit from Sprite and be loaded from SceneManager2D
	//
}


void GS_PlayState::KeyPress()
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::ESCAPE)) {
		Singleton<GameStateManager>::GetInstance()->setPaused(true);
	}
}

