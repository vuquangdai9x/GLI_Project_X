#pragma once
#include"StateBase.h"
#include"../Button.h"
#include<vector>
#include"../Misson.h"

class GS_PauseState : public StateBase
{
private:
	std::vector<Button*> button;
	Misson* misson;

public:
	GS_PauseState();
	~GS_PauseState();
	bool Create();
	bool Release();

	void Render();
	void Update(float deltaTime);

	void KeyPress();
	void SetMisson(Misson* misson) { this->misson = misson; }
};