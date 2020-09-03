#include "UnitButton.h"
#include "Singleton.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"SceneManager2D.h"
void UnitButton::setInformation(UnitInfor infor)
{
	m_infor = infor;
}

void UnitButton::setBuffer(UnitInfor** buf)
{
	buffer = buf;
}

void UnitButton::Update(float deltaTime)
{

	if (checkMouse()) {
		if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) {
			DWORD start = GetTickCount();
			if (start - m_click > 200) {
				this->getInformation();
				m_click = start;
			}
		}
		else {
			Sprite::Update(deltaTime);
			this->SetColor(0xffafff, 1);
		}
	}
	else {
		this->SetColor(0xffffff, 1);
	}
}

UnitButton::UnitButton(int id) :Button(id)
{

}

void UnitButton::getInformation()
{
	*buffer = &m_infor;
}
