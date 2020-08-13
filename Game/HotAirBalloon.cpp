#include "HotAirBalloon.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "../Framework3D/Utilities/utilities.h"
#include "Singleton.h"

HotAirBalloon::HotAirBalloon()
{
	this->m_moveSpeed = 5;
	m_cameraOffset.x = 0;
	m_cameraOffset.y = 0;
}

HotAirBalloon::~HotAirBalloon()
{
}

void HotAirBalloon::AssignSprite(int id) {
	m_sprite = &(Singleton<SceneManager2D>::GetInstance()->GetObjectByID(id));
}

void HotAirBalloon::Update(float deltaTime)
{
	if (m_sprite == NULL) return;

	Vector3 playerPos;
	playerPos.x = m_sprite->GetPosition().x;
	playerPos.y = m_sprite->GetPosition().y;
	playerPos.z = m_sprite->GetPosition().z;
	m_sprite->GetPosition();
	playerPos.y += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S)) * m_moveSpeed * deltaTime;
	playerPos.x += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A)) * m_moveSpeed * deltaTime;
	m_sprite->SetPosition(playerPos);

	Camera2D & camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 camPos = camera.GetPosition();
	camPos.x = m_sprite->GetPosition().x + m_cameraOffset.x;
	camPos.y = m_sprite->GetPosition().y + m_cameraOffset.y;
	camera.SetPosition(camPos);
}