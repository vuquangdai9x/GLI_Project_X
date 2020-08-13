#include "Player.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "../Framework3D/Utilities/utilities.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
void Player::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	playerBody = Singleton<WorldManager>::GetInstance()->createRectagle(PLAYER, x, y, 1.0f, 1.0f);
}
Player::Player(int id)
{
	//this->sprite =& Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);
	//Vector2 temp = Singleton<SceneManager2D>::GetInstance()->get2Dpos(this->sprite->GetPosition().x, this->sprite->GetPosition().y, this->sprite->GetPosition().z);
	//this->x = temp.x;
	//this->y = temp.y;
	Sprite::Sprite(id);
	this->m_moveSpeed = 5;

	m_cameraOffset.x = 0;
	m_cameraOffset.y = 0;
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	/*Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 cameraMoveDirection(0, 0, 0);
	cameraMoveDirection.x = Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A);
	cameraMoveDirection.y = -Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) + Singleton<InputManager>::GetInstance()->GetBit(InputManager::S);
	camera.Move(cameraMoveDirection, deltaTime);*/

	//camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Q), deltaTime);
	//camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Z), deltaTime);

	//Sprite& player = Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);

	Vector3 playerPos;
	//playerPos.x = this->GetPosition().x;
	//playerPos.y = this->GetPosition().y;
	playerPos.z = this->GetPosition().z;
	this->GetPosition();
	//playerPos.y += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S)) * m_moveSpeed * deltaTime;
	//playerPos.x += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A)) * m_moveSpeed * deltaTime;
	
	//float force_y = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S));
	//float force_x = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A));
	//playerBody->body->ApplyForce(b2Vec2(force_x, force_y), playerBody->body->GetWorldCenter(), TRUE);
	//if(pre_KeyY!=force_y) playerBody->body->ApplyLinearImpulseToCenter(b2Vec2(0.0, (force_y-pre_KeyY)*(Y_FORCE)), true);
	//if(pre_KeyX != force_x) playerBody->body->ApplyLinearImpulseToCenter(b2Vec2( (force_x - pre_KeyX) * X_FORCE,0.0), true);
	playerPos.x = playerBody->body->GetPosition().x;
	playerPos.y = playerBody->body->GetPosition().y;
	//float delta_x, delta_y;
	//delta_y = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S)) * m_moveSpeed * deltaTime;
	//delta_x = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A)) * m_moveSpeed * deltaTime;
	//playerBody->body->SetTransform(b2Vec2(playerPos.x + delta_x, playerPos.y + delta_y), 0.0f);
	

	this->SetPosition(playerPos);

	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 camPos = camera.GetPosition();
	camPos.x = this->GetPosition().x + m_cameraOffset.x;
	camPos.y = this->GetPosition().y + m_cameraOffset.y;
	camera.SetPosition(camPos);

	//pre_KeyX = force_x, pre_KeyY = force_y;
}

