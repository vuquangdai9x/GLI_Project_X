#include "stdafx.h"
#include "InputManager.h"
#include "SceneManager.h"

InputManager * InputManager::ms_pInstance = nullptr;

InputManager::InputManager()
{
	m_mask = 0;
}

inline void SetBit(unsigned int& mask, int offset)
{
	mask |= (1 << offset);
}
inline void UnsetBit(unsigned int& mask, int offset)
{
	mask &= (~(1 << offset));
}

inline int GetBit(unsigned int& mask, int offset)
{
	return (mask >> offset) & 0x01;
}

void InputManager::KeyPressed(unsigned char key, bool isKeyPressed)
{
	if (isKeyPressed) {
		switch (key)
		{
		case 'a':
		case 'A':
			SetBit(m_mask, Key::A);
			break;
		case 's':
		case 'S':
			SetBit(m_mask, Key::S);
			break;
		case 'd':
		case 'D':
			SetBit(m_mask, Key::D);
			break;
		case 'w':
		case 'W':
			SetBit(m_mask, Key::W);
			break;
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_SHIFT:
			SetBit(m_mask, Key::LSHIFT);
			break;
		case VK_SPACE:
			SetBit(m_mask, Key::SPACE);
			break;
		case VK_UP:
			SetBit(m_mask, Key::UP);
			break;
		case VK_DOWN:
			SetBit(m_mask, Key::DOWN);
			break;
		case VK_LEFT:
			SetBit(m_mask, Key::LEFT);
			break;
		case VK_RIGHT:
			SetBit(m_mask, Key::RIGHT);
			break;
		case '1':
			SetBit(m_mask, Key::N1);
			break;
		case '2':
			SetBit(m_mask, Key::N2);
			break;
		case '3':
			SetBit(m_mask, Key::N3);
			break;
		case '4':
			SetBit(m_mask, Key::N4);
			break;
		case 'q':
		case 'Q':
			SetBit(m_mask, Key::Q);
			break;
		case 'e':
		case 'E':
			SetBit(m_mask, Key::E);
			break;
		case VK_TAB:
			SetBit(m_mask, Key::TAB);
			break;
		}
	}
	else {
		switch (key)
		{
		case 'a':
		case 'A':
			UnsetBit(m_mask, Key::A);
			break;
		case 's':
		case 'S':
			UnsetBit(m_mask, Key::S);
			break;
		case 'd':
		case 'D':
			UnsetBit(m_mask, Key::D);
			break;
		case 'w':
		case 'W':
			UnsetBit(m_mask, Key::W);
			break;
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_SHIFT:
			UnsetBit(m_mask, Key::LSHIFT);
			break;
		case VK_SPACE:
			UnsetBit(m_mask, Key::SPACE);
			break;
		case VK_UP:
			UnsetBit(m_mask, Key::UP);
			break;
		case VK_DOWN:
			UnsetBit(m_mask, Key::DOWN);
			break;
		case VK_LEFT:
			UnsetBit(m_mask, Key::LEFT);
			break;
		case VK_RIGHT:
			UnsetBit(m_mask, Key::RIGHT);
			break;
		case '1':
			UnsetBit(m_mask, Key::N1);
			break;
		case '2':
			UnsetBit(m_mask, Key::N2);
			break;
		case '3':
			UnsetBit(m_mask, Key::N3);
			break;
		case '4':
			UnsetBit(m_mask, Key::N4);
			break;
		case 'q':
		case 'Q':
			UnsetBit(m_mask, Key::Q);
			break;
		case 'e':
		case 'E':
			UnsetBit(m_mask, Key::E);
			break;
		case VK_TAB:
			UnsetBit(m_mask, Key::TAB);
			break;
		}
	}
}

void InputManager::Update(float deltaTime) {
	Camera& camera = SceneManager::GetInstance()->GetMainCamera();
	Vector3 cameraMoveDirection(0,0,0);
	cameraMoveDirection.x = GetBit(m_mask, Key::D) - GetBit(m_mask, Key::A);
	cameraMoveDirection.z = GetBit(m_mask, Key::W) - GetBit(m_mask, Key::S);
	camera.MoveByLocalAxis(cameraMoveDirection,deltaTime);

	camera.MoveByWorldAxis(Vector3(0, GetBit(m_mask, Key::SPACE) - GetBit(m_mask, Key::LSHIFT), 0), deltaTime);

	Vector3 cameraRotateDirection(0, 0, 0);
	cameraRotateDirection.x = GetBit(m_mask, Key::DOWN) - GetBit(m_mask, Key::UP);
	cameraRotateDirection.y = GetBit(m_mask, Key::RIGHT) - GetBit(m_mask, Key::LEFT);
	camera.Rotate(cameraRotateDirection.y,cameraRotateDirection.x,deltaTime);

	if (GetBit(m_mask, Key::TAB) == 1) {
		camera.SetVectorUp(Vector3(0, 1, 0));
	}
	else {
		camera.Dutch(GetBit(m_mask, Key::E) - GetBit(m_mask, Key::Q), deltaTime);
	}

	SceneManager::GetInstance()->m_fogStart += (GetBit(m_mask, Key::N2) - GetBit(m_mask, Key::N1));
	SceneManager::GetInstance()->m_fogLength += (GetBit(m_mask, Key::N4) - GetBit(m_mask, Key::N3));
}