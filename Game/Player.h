#pragma once
#include "Sprite.h"
#include "ItemBody.h"
#include "HUDController.h"
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

	DWORD m_TakeDameTime = 0;
	int m_immortalTime = 2000;
	int m_currentColor = 0;
	int m_color[2] = { 0xffafff, 0x4863A0 };

	int m_HP, m_maxHP;
	int m_damage;
	float m_moveSpeed, m_flySpeed;
	int m_score;
	
	ItemBody* playerBody;
	Vector2 m_cameraOffset;
	HUDController* m_HUDController;
	int time = 0;
	DWORD m_timeEnd = 0;

	float durationDamage;
	float durationSpeed;

public:
	Player(int id);
	~Player();
	void Update(float deltaTime);
	void createBox2D();
	void setFlyState(FlyState fly);
	void updateFlyState();
	void setMoveState(MoveState move);
	void updateMoveState();
	void setHP(int hp) { this->m_HP = hp; }
	int getHP() { return this->m_HP; }
	int getMaxHP() { return this->m_maxHP; }
	void setDamage(int dmg) { this->m_damage = dmg; }
	int getDamage() { return this->m_damage; }
	void setScore(int score) { this->m_score = score; }
	int getScore() { return this->m_score; }
	void setMoveSpeed(float moveSpeed) { this->m_moveSpeed = moveSpeed; }
	float getMoveSpeed() { return this->m_moveSpeed; }
	void setFlySpeed(float flySpeed) { this->m_flySpeed = flySpeed; }
	float getFlySpeed() { return this->m_flySpeed; }
	void setDurationSpeed(float duration) { this->durationSpeed = duration; }
	void setDurationDamage(float duration) { this->durationDamage = duration; }

	void setHUDController(HUDController* hud) { this->m_HUDController = hud; }
	HUDController* getHUDController() { return this->m_HUDController; }
};

