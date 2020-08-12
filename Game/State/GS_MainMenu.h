#pragma once

#include"StateBase.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include<stdio.h>
class GS_MainMenu: public StateBase
{
public:
	GS_MainMenu();
	~GS_MainMenu();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

