#pragma once

#include"StateBase.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include"../SceneManager2D.h"
#include"../ResourcesManager2D.h"

#include<stdio.h>
class GS_PlayState: public StateBase
{
private:
	//Player player;
public:
	GS_PlayState();
	~GS_PlayState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

