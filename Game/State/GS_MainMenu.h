#pragma once

#include"StateBase.h"
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

