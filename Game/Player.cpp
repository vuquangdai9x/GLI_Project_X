#include "Player.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "SceneManager2D.h"
#include "ResourcesManager2D.h"
#include "Singleton.h"
Player::Player()
{
	//this->sprite =& Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);
	//Vector2 temp = Singleton<SceneManager2D>::GetInstance()->get2Dpos(this->sprite->GetPosition().x, this->sprite->GetPosition().y, this->sprite->GetPosition().z);
	//this->x = temp.x;
	//this->y = temp.y;
	this->moveSpeed = 5;
}

Player::~Player()
{
}

void Player::update(float deltaTime)
{
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 cameraMoveDirection(0, 0, 0);
	cameraMoveDirection.x = Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A);
	cameraMoveDirection.y = -Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) + Singleton<InputManager>::GetInstance()->GetBit(InputManager::S);
	camera.Move(cameraMoveDirection, deltaTime);

	//camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Q), deltaTime);
	//camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Z), deltaTime);

	//Sprite& player = Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);

	Vector3 playerPos;
	playerPos.x = this->GetPosition().x;
	playerPos.y = this->GetPosition().y;
	playerPos.z = this->GetPosition().z;
	this->GetPosition();
	playerPos.y += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::S) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::W)) * moveSpeed * deltaTime;
	playerPos.x += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A)) * moveSpeed * deltaTime;
	this->SetPosition(playerPos);
}

void Player::render()
{
	Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera());
	
}

void Player::KeyPress()
{
}
