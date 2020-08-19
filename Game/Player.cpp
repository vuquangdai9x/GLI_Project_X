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
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	m_currentMoveSpeed = 0.0f;m_desireMoveSpeed = 0.0f;
	m_currentFlySpeed = Y_FORCE;m_desireFlySpeed = Y_FORCE;
	playerBody = Singleton<WorldManager>::GetInstance()->createRectagle(PLAYER, x, y, width, height);
	
	//playerBody->body->ApplyForceToCenter(b2Vec2(0.0f, GRAVITY * DEFAULT_MASS), false);
}
void Player::setFlyState(FlyState fly)
{
	switch (fly)
	{
	case Fast:
		m_desireFlySpeed = 10.0f;
		break;
	case Normal:
		m_desireFlySpeed = 5.0f;
		break;
	case SLow:
		m_desireFlySpeed = 2.0f;
		break;
	case Static:
		m_desireFlySpeed = 0.0f;
		break;
	default:
		break;
	}
	updateFlyState();
}
void Player::updateFlyState()
{
	m_flyForce = m_desireFlySpeed - this->playerBody->body->GetLinearVelocityFromWorldPoint(playerBody->body->GetWorldCenter()).y;
	if (abs(m_flyForce) > maxForce) {
		if (m_flyForce > 0) m_flyForce = maxForce;
		else m_flyForce = -maxForce;
	}
	playerBody->body->ApplyLinearImpulseToCenter(b2Vec2(0.0, m_flyForce * playerBody->body->GetMass()), false);
}
void Player::setMoveState(MoveState move)
{
	switch (move)
	{
	case Left:
		m_desireMoveSpeed = -5.0;
		break;
	case Right:
		m_desireMoveSpeed = 5.0;
		break;
	case NonMove:
		m_desireMoveSpeed = 0.0;
		break;
	default:
		break;
	}
	updateMoveState();
}
void Player::updateMoveState()
{
	m_moveForce = m_desireMoveSpeed - this->playerBody->body->GetLinearVelocityFromWorldPoint(playerBody->body->GetWorldCenter()).x;
	if (abs(m_moveForce) > maxForce) {
		if (m_moveForce > 0) m_moveForce = maxForce;
		else m_moveForce = -maxForce;
	}
	playerBody->body->ApplyLinearImpulseToCenter(b2Vec2( m_moveForce * playerBody->body->GetMass(),0.0), false);
}
Player::Player(int id): Sprite(id)
{
	//this->sprite =& Singleton<SceneManager2D>::GetInstance()->GetObjectByID(0);
	//Vector2 temp = Singleton<SceneManager2D>::GetInstance()->get2Dpos(this->sprite->GetPosition().x, this->sprite->GetPosition().y, this->sprite->GetPosition().z);
	//this->x = temp.x;
	//this->y = temp.y;
	this->m_moveSpeed = 5;

	m_cameraOffset.x = 0;
	m_cameraOffset.y = 0;
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	Vector3 playerPos;
	playerPos.z = this->GetPosition().z;
	this->GetPosition();

	float key_y = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S));
	float key_x = (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A));
	if (key_y == 1) time++;
	if (time == 0) {
		playerPos.x = playerBody->body->GetPosition().x;
		playerPos.y = playerBody->body->GetPosition().y;
		this->SetPosition(playerPos);
		Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
		Vector3 camPos = camera.GetPosition();
		camPos.x = this->GetPosition().x + m_cameraOffset.x;
		camPos.y = this->GetPosition().y + m_cameraOffset.y;
		camera.SetPosition(camPos);

		//b2Vec2 v = playerBody->body->GetLinearVelocityFromWorldPoint(playerBody->body->GetWorldCenter());
		//this->SetPosition(playerPos);
		//printf("%f %f \n", v.x, v.y);

		return;
	};
	if (time == 1) {
		playerBody->body->SetLinearVelocity(b2Vec2(0.0, Y_FORCE));
		return;
	}

	setFlyState(FlyState((int)key_y));
	setMoveState(MoveState((int)key_x));
	playerPos.x = playerBody->body->GetPosition().x;
	playerPos.y = playerBody->body->GetPosition().y;

	b2Vec2 v = playerBody->body->GetLinearVelocityFromWorldPoint(playerBody->body->GetWorldCenter());
	this->SetPosition(playerPos);
	printf("%f %f \n", v.x, v.y);
	
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 camPos = camera.GetPosition();
	camPos.x = this->GetPosition().x + m_cameraOffset.x;
	camPos.y = this->GetPosition().y + m_cameraOffset.y;
	camera.SetPosition(camPos);
}

