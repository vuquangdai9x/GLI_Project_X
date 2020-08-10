#pragma once
#include"StateBase.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include<stdio.h>
class GS_WelcomScreen: public StateBase
{
public:
	GS_WelcomScreen();
	~GS_WelcomScreen();
	bool Create();
	bool Release();

	void Render();
	void Update();

	void KeyPress(unsigned char key, bool bIsPressed);
};

