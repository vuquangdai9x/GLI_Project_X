#pragma once

#include"StateBase.h"
#include"../Button.h"
class GS_GameOverState : public StateBase
{
private:
	std::vector<Button*> button;
public:
	GS_GameOverState();
	~GS_GameOverState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
};