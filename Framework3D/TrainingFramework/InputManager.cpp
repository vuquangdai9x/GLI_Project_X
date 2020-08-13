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

void InputManager::Update(float deltaTime) {
	/*Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 cameraMoveDirection(0,0,0);
	cameraMoveDirection.x = GetBit(Key::D) - GetBit(Key::A);
	cameraMoveDirection.y = -GetBit(Key::W) + GetBit(Key::S);
	camera.Move(cameraMoveDirection,deltaTime);

	camera.Dutch(GetBit(Key::E) - GetBit(Key::Q), deltaTime);
	camera.Zoom(GetBit(Key::X) - GetBit(Key::Z), deltaTime);

	Sprite& player = Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);

	float moveSpeed = 5;
	Vector3 playerPos = player.GetPosition();
	playerPos.y += (GetBit(Key::S) - GetBit(Key::W)) * moveSpeed * deltaTime;
	playerPos.x += (GetBit(Key::D) - GetBit(Key::A)) * moveSpeed * deltaTime;
	player.SetPosition(playerPos);
	Vector2 playerScale = player.GetScale();
	playerScale.x += (GetBit(Key::N2) - GetBit(Key::N1)) * moveSpeed * deltaTime;
	playerScale.y += (GetBit(Key::N4) - GetBit(Key::N3)) * moveSpeed * deltaTime;
	player.SetScale(playerScale);
	float rotation = player.GetRotation();
	rotation += (GetBit(Key::SPACE) - GetBit(Key::LSHIFT)) * moveSpeed * deltaTime;
	player.SetRotation(rotation);*/
	
	/*Camera& camera = SceneManager::GetInstance()->GetMainCamera();
	Vector3 cameraMoveDirection(0,0,0);
	cameraMoveDirection.x = GetBit(Key::D) - GetBit(Key::A);
	cameraMoveDirection.z = GetBit(Key::W) - GetBit(Key::S);
	camera.MoveByLocalAxis(cameraMoveDirection,deltaTime);

	camera.MoveByWorldAxis(Vector3(0, GetBit(Key::SPACE) - GetBit(Key::LSHIFT), 0), deltaTime);

	Vector3 cameraRotateDirection(0, 0, 0);
	cameraRotateDirection.x = GetBit(Key::DOWN) - GetBit(Key::UP);
	cameraRotateDirection.y = GetBit(Key::RIGHT) - GetBit(Key::LEFT);
	camera.Rotate(cameraRotateDirection.y,cameraRotateDirection.x,deltaTime);

	if (GetBit(Key::TAB) == 1) {
		camera.SetVectorUp(Vector3(0, 1, 0));
	}
	else {
		camera.Dutch(GetBit(Key::E) - GetBit(Key::Q), deltaTime);
	}
	SceneManager::GetInstance()->m_fogStart += (GetBit(Key::N2) - GetBit(Key::N1));
	SceneManager::GetInstance()->m_fogLength += (GetBit(Key::N4) - GetBit(Key::N3));*/
}