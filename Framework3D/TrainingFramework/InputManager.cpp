#include "stdafx.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Singleton.h"

//InputManager * InputManager::ms_pInstance = nullptr;

InputManager::InputManager()
{
	m_mask = 0;
}

int InputManager::GetBit(Key key) {
	return (m_mask >> key) & 0x01;
}

void InputManager::MouseEvent(int typeOfService, int button, int x, int y)
{
	m_mouseEvent = typeOfService;
	this->x = x;
	this->y = y;
	this->button = button;
}

int InputManager::getMouseButton()
{
	return button;
}

int InputManager::getMouseEvent()
{
	return m_mouseEvent;
}

void InputManager::getXY(int& xx, int& yy)
{
	xx = x;
	yy = y;
}

void InputManager::KeyPressed(unsigned char key, bool isKeyPressed)
{
	Key keyMapped;
	switch (key)
	{
	case 'a':
	case 'A':
		keyMapped = Key::A;
		break;
	case 's':
	case 'S':
		keyMapped = Key::S;
		break;
	case 'd':
	case 'D':
		keyMapped = Key::D;
		break;
	case 'w':
	case 'W':
		keyMapped = Key::W;
		break;
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_SHIFT:
		keyMapped = Key::LSHIFT;
		break;
	case VK_SPACE:
		keyMapped = Key::SPACE;
		break;
	case VK_UP:
		keyMapped = Key::UP;
		break;
	case VK_DOWN:
		keyMapped = Key::DOWN;
		break;
	case VK_LEFT:
		keyMapped = Key::LEFT;
		break;
	case VK_RIGHT:
		keyMapped = Key::RIGHT;
		break;
	case '1':
		keyMapped = Key::N1;
		break;
	case '2':
		keyMapped = Key::N2;
		break;
	case '3':
		keyMapped = Key::N3;
		break;
	case '4':
		keyMapped = Key::N4;
		break;
	case '5':
		keyMapped = Key::N5;
		break;
	case '6':
		keyMapped = Key::N6;
		break;
	case '7':
		keyMapped = Key::N7;
		break;
	case '8':
		keyMapped = Key::N8;
		break;
	case '9':
		keyMapped = Key::N9;
		break;
	case '0':
		keyMapped = Key::N0;
		break;
	case 'q':
	case 'Q':
		keyMapped = Key::Q;
		break;
	case 'e':
	case 'E':
		keyMapped = Key::E;
		break;
	case VK_TAB:
		keyMapped = Key::TAB;
		break;
	case 'Z':
	case 'z':
		keyMapped = Key::Z;
		break;
	case 'X':
	case 'x':
		keyMapped = Key::X;
		break;
	case 'C':
	case 'c':
		keyMapped = Key::C;
		break;
	default:
		keyMapped = Key::NONE;
		return;
	}
	if (isKeyPressed) {
		m_mask |= (1 << keyMapped);
	}
	else {
		m_mask &= (~(1 << keyMapped));
	}
}