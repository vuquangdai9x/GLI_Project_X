#pragma once
#include "UIComponent.h"
class Button : public UIComponent
{
protected:
	float x, y;
	float width, height;
	float __w, __h;
	void(__cdecl* Click)();
	DWORD m_click = 0;
public:
	Button(int id);
	~Button();
	void UpdateMember();
	void Update(float deltaTime);
	void __cdecl OnClick(void(__cdecl* Click)());
	bool checkMouse();
};

