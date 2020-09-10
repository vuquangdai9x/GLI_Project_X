#include "../stdafx.h"
#include "GS_PlayState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../EffectManager.h"
#include "../BrokenBalloon.h"

GS_PlayState::GS_PlayState()
{
    printf("GS_PlayState\n");
	// 2D
	//char sceneFile[50] = "Datas/scene2d.txt";
	char sceneFile[50];
	int map = Singleton<GameStateManager>::GetInstance()->getMap();
	snprintf(sceneFile, sizeof(sceneFile), "Datas/map1-%d.txt",map);
	//char sceneFile[50] = "Datas/map1-2.txt";
	char resourcesFile[50] = "Datas/resources2d.txt";
	char effectFile[50] = "Datas/effects.txt";

	//Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);
	Singleton<EffectManager>::GetInstance()->LoadEffect(effectFile);
	if (!Singleton<SceneManager2D>::GetInstance()->LoadScene(sceneFile)) {
		printf("[ERR] Entry point: Failed to init scene");
		return ;
	}

	/*if (!Singleton<SceneManager2D>::GetInstance()->LoadScene("map-1-3.txt")) {
		printf("[ERR] Entry point: Failed to init scene");
		return;
	}*/
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
	time++;
	if (time % 1000 == 0) {
		int type = rand() % 3;
		int size = rand() % 3 + 1;
		BrokenBalloon* balloon = new BrokenBalloon(-1, type, size);
		Singleton<SceneManager2D>::GetInstance()->AddObject(balloon);
	}
	
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Q), deltaTime);
	camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Z), deltaTime);

	// NOTE: for debug purpose. Player/HotAirBalloon must inherit from Sprite and be loaded from SceneManager2D
	//
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	DWORD start = GetTickCount();
	if (playerPos.y > Singleton<SceneManager2D>::GetInstance()->getHeightWin()) {
		if (this->m_PassTime == 0) this->m_PassTime = GetTickCount();
		Vector3 pos;
		pos.x = -60 + rand() % (60 + 1 - (-60));
		pos.y = -80 + rand() % (80 + 1 - (-80));
		pos.z = 0;
		Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(pos, 12100);
	}
	if (m_PassTime != 0 && (start - m_PassTime) > 2000) {
		Singleton<GameStateManager>::GetInstance()->Pop();
		Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PASSLEVEL);
	}
}


void GS_PlayState::KeyPress()
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::ESCAPE)) {
		Singleton<GameStateManager>::GetInstance()->setPaused(true);
		//Singleton<GameStateManager>::GetInstance()->getPauseState()->SetMisson(this->misson);
		Singleton<GameStateManager>::GetInstance()->getPauseState()->SetMisson(Singleton<SceneManager2D>::GetInstance()->getPlayer()->getMisson());
	}
}


