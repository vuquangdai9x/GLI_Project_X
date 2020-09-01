#include "Player.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "../Framework3D/Utilities/utilities.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include<math.h>
#include "GunBullet.h"
#include"UserData.h"
#include"State/GameStateManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

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
		m_desireFlySpeed = 10.0f + this->m_flySpeed;
		break;
	case Normal:
		m_desireFlySpeed = 5.0f + this->m_flySpeed;
		break;
	case SLow:
		m_desireFlySpeed = 1.0f + this->m_flySpeed;
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
		m_desireMoveSpeed = -5.0 - this->m_moveSpeed;
		break;
	case Right:
		m_desireMoveSpeed = 5.0 + this->m_moveSpeed;
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
	playerBody->body->ApplyLinearImpulseToCenter(b2Vec2(m_moveForce * playerBody->body->GetMass(), 0.0), false);
}

Player::Player(int id): Sprite(id)
{
	this->m_moveSpeed = 0;
	this->m_flySpeed = 0;

	m_cameraOffset.x = 0;
	m_cameraOffset.y = 0;

	m_HP = m_maxHP = 100;
	this->m_damage = 0;
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	Sprite::Update(deltaTime);

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
	//printf("%f %f \n", v.x, v.y);
	
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera();
	Vector3 camPos = camera.GetPosition();
	camPos.x = this->GetPosition().x + m_cameraOffset.x;
	camPos.y = this->GetPosition().y + m_cameraOffset.y;
	camera.SetPosition(camPos);

	UserData* user = (UserData*)this->playerBody->body->GetUserData();
	DWORD start;
	start = GetTickCount();
	if (user->IsCollison > 0) {
		if (start - m_TakeDameTime > m_immortalTime) {
			m_TakeDameTime = GetTickCount();
			if (user->m_receiveDamage) {
				Singleton<SoundManager>::GetInstance()->Player(SoundManager::P_INJUIRED);
			}
			this->m_HP -= user->m_receiveDamage;
			if (this->m_HP <= 0) {
				//m_HP = 0;
				Singleton<SoundManager>::GetInstance()->Player(SoundManager::P_DIED);
				Singleton<GameStateManager>::GetInstance()->Pop();
				Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::GAMEOVER);
			}
			Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(m_position, 10000);
		}
		this->SetColor(m_color[(++m_currentColor) % 2], 1);
	}

	else {
		if (start - m_TakeDameTime > m_immortalTime) this->SetColor(0xffffff, 1);
		else {
			this->SetColor(m_color[(++m_currentColor) % 2], 1);
		}
	}
	m_score = playerPos.y;
	this->m_HUDController->UpdateScore(m_score);
	this->m_HUDController->UpdateHealthBar(this->m_HP, this->m_maxHP);

	if (durationDamage > 0) {
		durationDamage -= deltaTime;
		if (durationDamage <= 0) m_damage = 0;
	}

	if (durationSpeed > 0) {
		durationSpeed -= deltaTime;
		if (durationSpeed <= 0) {
			m_moveSpeed = 0;
			m_flySpeed = 0;
		}
	}

	//printf("%f \n", getFireAngle());
	//if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) testCanon();
}

