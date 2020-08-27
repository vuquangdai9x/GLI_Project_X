#pragma once
#include"StateBase.h"
#include"../Button.h"
#include<vector>

class GS_PauseState : public StateBase
{
private:
	std::vector<Button*> button;

public:
	GS_PauseState();
	~GS_PauseState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();

};