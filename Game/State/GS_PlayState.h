#pragma once
#include"StateBase.h"
#include"../Misson.h"

class GS_PlayState: public StateBase
{
private:
	int time = 0;
	DWORD m_PassTime = 0;
public:
	GS_PlayState();
	~GS_PlayState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

