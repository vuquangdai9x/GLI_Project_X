#include "Button.h"
#include "Singleton.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"SceneManager2D.h"
void doNothing() {
}
Button::Button()
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

void __cdecl Button::OnClick(void(__cdecl* Click)())
{
	this->Click = Click;
}

bool Button::checkMouse()
{
	int xx, yy;
	Singleton<InputManager>::GetInstance()->getXY(xx, yy);
	Vector3 worldMouse=Singleton<SceneManager2D>::GetInstance()->get3Dpos(xx, yy,MENU_OBJECT);
	Vector3 thisPos = this->GetPosition();
	if (worldMouse.x > thisPos.x - __w && worldMouse.x< thisPos.x + __w && worldMouse.y>thisPos.y - __h && worldMouse.y < thisPos.y + __h) return true;
	return false;
}
