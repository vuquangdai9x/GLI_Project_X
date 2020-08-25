#include "../stdafx.h"
#include "GS_MainMenu.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../SoundManager.h"

GS_MainMenu::GS_MainMenu()
{
	Singleton<SoundManager>::GetInstance()->Init();
    printf("GS_MainMenu\n");
	char sceneFile[50] = "Datas/scene2d-menu.txt";
	char resourcesFile[60] = "Datas/resources2d.txt";

	Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);

	if (!Singleton<SceneManager2D>::GetInstance()->LoadMenuScene(sceneFile)) {
		printf("[ERR] Entry point: Failed to init scene");
		return;
	}
}

GS_MainMenu::~GS_MainMenu()
{
}

bool GS_MainMenu::Create()
{
    return false;
}

void GS_MainMenu::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render(MENU_OBJECT);
}
void GS_MainMenu::Update(float deltaTime)
{
	Singleton<SceneManager2D>::GetInstance()->Update(deltaTime, MENU_OBJECT);
}

bool GS_MainMenu::Release()
{
    return false;
}

void GS_MainMenu::KeyPress()
{
}

