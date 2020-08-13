#pragma once
#include"StateBase.h"
#include "../HotAirBalloon.h"

class GS_PlayState: public StateBase
{
private:
	// NOTE: for debug purpose. Player/HotAirBalloon must inherit from Sprite and be loaded from SceneManager2D
	HotAirBalloon m_player;
	//

public:
	GS_PlayState();
	~GS_PlayState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};

