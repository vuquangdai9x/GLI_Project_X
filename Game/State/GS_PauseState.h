#pragma once
#include"StateBase.h"

class GS_PauseState : public StateBase
{
private:
	// NOTE: for debug purpose. Player/HotAirBalloon must inherit from Sprite and be loaded from SceneManager2D
	//

public:
	GS_PauseState();
	~GS_PauseState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};