#pragma once
#include "Sprite.h"
#include "ItemBody.h"
enum FlyState { Fast = 1, Normal = 0, SLow = -1, Static = 2 };
enum MoveState { Left = -1, Right = 1, NonMove = 0 };
class Player : public Sprite
{
private:
	float x, y;
	float width, height;

	float m_currentMoveSpeed, m_desireMoveSpeed;
	float m_currentFlySpeed, m_desireFlySpeed;
	float m_flyForce, m_moveForce;
	float maxForce = 0.2f;

	float m_flyMulti, m_moveMulti;

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed;
	ItemBody* playerBody;
	Vector2 m_cameraOffset;
	Sprite* m_target;
	int time = 0;
	DWORD m_timeEnd = 0;

public:
	Player(int id);
	~Player();
	void Update(float deltaTime);
	void createBox2D();
	void setFlyState(FlyState fly);
	void updateFlyState();
	void setMoveState(MoveState move);
	void updateMoveState();
	void setTarget(Sprite* target);
	float getFireAngle();
	void testCanon();
	void setHP(int hp) { this->m_HP = hp; }
	int getHP() { return this->m_HP; }
};

