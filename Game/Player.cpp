#include "Player.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "../Framework3D/Utilities/utilities.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include<math.h>
#include "GunBullet.h"

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
		m_desireFlySpeed = -5.0f;
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
	playerBody->body->ApplyLinearImpulseToCenter(b2Vec2(m_moveForce * playerBody->body->GetMass(), 0.0), false);
}
void Player::setTarget(Sprite* target)
{
	m_target = target;
}
//float Player::getFireAngle()
//{
//	Vector3 playerPos, tagetPos;
//	playerPos = this->GetPosition();
//	tagetPos = m_target->GetPosition();
//	Vector2 rootVec(0.0, tagetPos.y - playerPos.y);
//	Vector2 current(tagetPos.x - playerPos.x, tagetPos.y - playerPos.y);
//	float cos_angle = (rootVec.Length()) / (current.Length());
//	if (tagetPos.x > playerPos.x) {
//		if (tagetPos.y > playerPos.y)
//			return -(acos(cos_angle));
//		return -(asinf(cos_angle)+M_PI_2);
//	}
//	else {
//		if (tagetPos.y > playerPos.y)
//			return (acos(cos_angle));
//		return (asinf(cos_angle) + M_PI_2);
//	}
//	
//
//}
//void Player::testCanon()
//{
//	DWORD start = GetTickCount();
//
//	if (start-m_timeEnd >=1000) {
//		Vector3 playerPos, tagetPos;
//		playerPos = this->GetPosition();
//		tagetPos = m_target->GetPosition();
//		float angle = getFireAngle() * 180 / M_PI ;
//		printf("%f \n", angle);
//		if (tagetPos.x > playerPos.x) {
//			GunBullet* gun = new GunBullet(b2Vec2(playerPos.x + 1.0, playerPos.y), angle);
//			//CanonBullet* gun = new CanonBullet(b2Vec2(playerPos.x + 1.0, playerPos.y), angle+30); // canon thi cong 30 trong that hon
//			Singleton<SceneManager2D>::GetInstance()->AddObject(gun);
//		}
//		else {
//			GunBullet* gun = new GunBullet(b2Vec2(playerPos.x - 1.0, playerPos.y), angle);
//			//CanonBullet* gun = new CanonBullet(b2Vec2(playerPos.x - 1.0, playerPos.y), angle-30);
//			Singleton<SceneManager2D>::GetInstance()->AddObject(gun);
//		}
//		//GunBullet* gun = new GunBullet(b2Vec2(playerPos.x+1.5, playerPos.y), angle);
//		//CanonBullet* gun = new CanonBullet(b2Vec2(playerPos.x + 1.5, playerPos.y), angle);
//		
//		m_timeEnd= start;
//	}
//
//	
//}
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

	Vector3 targetPos;
	int i_x, i_y;
	float f_x, f_y;
	Singleton<InputManager>::GetInstance()->getXY(i_x, i_y);
	f_x = i_x;f_y = i_y;
	targetPos = Singleton<SceneManager2D>::GetInstance()->get3Dpos(f_x, f_y);
	targetPos.z = playerPos.z + 1.0f;
	m_target->SetPosition(targetPos);

	//printf("%f \n", getFireAngle());
	//if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) testCanon();
}

