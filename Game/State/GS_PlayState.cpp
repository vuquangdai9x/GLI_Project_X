#include "GS_PlayState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"

GS_PlayState::GS_PlayState()
{
    printf("GS_PlayState\n");
	// 2D
	char sceneFile[50] = "../Framework3D/Resources/Datas/scene2d.txt";
	char resourcesFile[50] = "../Framework3D/Resources/Datas/resources2d.txt";

	Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);

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
}

void GS_PlayState::KeyPress()
{
}

