#include "GS_WelcomScreen.h"

GS_WelcomScreen::GS_WelcomScreen()
{
    printf("GS_WelcomScreen\n");
}

GS_WelcomScreen::~GS_WelcomScreen()
{
}

bool GS_WelcomScreen::Create()
{
    return false;
}

bool GS_WelcomScreen::Release()
{
    return false;
}

void GS_WelcomScreen::Render()
{
}

void GS_WelcomScreen::Update()
{
}

void GS_WelcomScreen::KeyPress(unsigned char key, bool bIsPressed)
{
    if (bIsPressed) {
        switch (key)
        {
        case 'a': 
        case 'A': {
            Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::MENU);
            break;
        }
        default:
            break;
        }
    }
}

