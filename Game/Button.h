#pragma once
#include "Sprite.h"
class Button : public Sprite
{
private:
	float x, y;
	float width, height;
	void(__cdecl* Click)();
	DWORD m_click = 0;
public:
	Button();
	~Button();
	void Update(float deltaTime);
	void __cdecl OnClick(void(__cdecl* Click)());
	bool checkMouse();
};

