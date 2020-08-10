#pragma once
#include"StateBase.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include<stdio.h>
class GS_PlayState: public StateBase
{
public:
	GS_PlayState();
	~GS_PlayState();
	bool Create();
	bool Release();

	void Render();
	void Update();

	void KeyPress(unsigned char key, bool bIsPressed);
};

