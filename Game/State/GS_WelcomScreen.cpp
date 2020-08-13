#include "../stdafx.h"
#include "GS_WelcomScreen.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../Framework3D/TrainingFramework/framework3d.h"

GS_WelcomScreen::GS_WelcomScreen()
{
    printf("GS_WelcomScreen\n");
	printf("Press A to init scene\n");
}

GS_WelcomScreen::~GS_WelcomScreen()
{
}

bool GS_WelcomScreen::Create()
{
    return false;
}

void GS_WelcomScreen::Render()
{
}
void GS_WelcomScreen::Update(float deltaTime)
{

}

bool GS_WelcomScreen::Release()
{
    return false;
}

void GS_WelcomScreen::KeyPress()
{
    if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::A)) {
        Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
    }
}

