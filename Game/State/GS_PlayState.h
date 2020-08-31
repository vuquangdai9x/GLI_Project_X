#pragma once
#include"StateBase.h"

class GS_PlayState: public StateBase
{
private:
	int time = 0;

public:
	GS_PlayState();
	~GS_PlayState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

