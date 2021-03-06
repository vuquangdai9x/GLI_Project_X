#pragma once
#include "Box2D/Box2D.h"
#define PLAYER 0
#define OBSTACLE 1
#define ENEMY 2
#define PLAYERBULLET 3
#define ENEMYBULLET 4
#define SPECIAL_ENEMY 5
#define MAP_BORDER 6
#define ITEM 7
#define DECORATE_OBJ 8

#define DEFAULT_MASS 5.0f
#define GRAVITY 10.0f
#define MOVING_FORCE (15.0f*DEFAULT_MASS)
#define Y_FORCE 5.0f
#define X_FORCE 5.0f


class ItemBody
{
public:
	ItemBody(int type,float x, float y);
	~ItemBody();
	b2Body* body;
	void Update(float deltaTime);
	void SetGravityScale(float value);
	void setActive(bool active);
	int GetType() { return this->m_type; }
	bool getActive() { return m_active; }
private:
	int m_type;
	float m_x, m_y;
	bool m_active=true;
	int firstTime = 0;
	float pre_KeyX = 0.0f;
	float pre_KeyY = 0.0f;

	float m_gravityScale;
};

