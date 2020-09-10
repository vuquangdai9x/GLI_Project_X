#include "Button.h"
#include "Singleton.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"SceneManager2D.h"

void doNothing() {
}
Button::Button(int id):UIComponent(id)
{
	this->Click = doNothing;
}

Button::~Button()
{
}

void Button::UpdateMember()
{
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	 __w = width / 2, __h = height / 2;
}

void Button::Update(float deltaTime)
{
	if (checkMouse()) {
		if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) {
			DWORD start = GetTickCount();
			if (start - m_click > 200) {
				this->Click();
				m_click = start;
			}
		}
		else {
			this->SetColor(0xffafff, 1);
		}
	}
	else {
		this->SetColor(0xffffff, 1);
	}
}

bool Button::Update()
{
	if (checkMouse()) {
		if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) {
			DWORD start = GetTickCount();
			if (start - m_click > 200) {
				this->Click();
				m_click = start;
				return true;
			}
		}
		else {
			this->SetColor(0xffafff, 1);
		}
	}
	else {
		this->SetColor(0xffffff, 1);
	}
	return false;
}

void __cdecl Button::OnClick(void(__cdecl* Click)())
{
	this->Click = Click;
}

bool Button::checkMouse()
{
	int iMousePosX, iMousePosY;
	Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
	// chuyen ve toa do [-1..1]
	float mousePosX = iMousePosX / (float)Globals::screenWidth * 2 - 1;
	float mousePosY = iMousePosY / (float)Globals::screenHeight * 2 - 1;
	mousePosY *= -1; // toa do chuot nguoc voi toa do UI

	Vector4 bottomLeft(-1, -1, 0, 1);
	Vector4 topRight(1, 1, 0, 1);
	bottomLeft =  bottomLeft * m_WVP;
	topRight = topRight * m_WVP;

	return ((bottomLeft.x < mousePosX&& mousePosX < topRight.x) && (bottomLeft.y < mousePosY&& mousePosY < topRight.y));
}
