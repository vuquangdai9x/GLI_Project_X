#include "Bullet.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
Bullet::Bullet()
{
}
Bullet::Bullet(b2Vec2 startPosition)
{
	Vector3 bulletPos;
	bulletPos.x = startPosition.x;
	bulletPos.y = startPosition.y;
	bulletPos.z = 0;
}

Bullet::~Bullet()
{
}

void Bullet::createBox2D()
{
}

void Bullet::Fire(b2Vec2 direction)
{
}

void Bullet::Update(float deltaTime)
{
	
}
