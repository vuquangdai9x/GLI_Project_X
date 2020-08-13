#pragma once
#include"StateBase.h"
class GS_WelcomScreen: public StateBase
{
public:
	GS_WelcomScreen();
	~GS_WelcomScreen();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

