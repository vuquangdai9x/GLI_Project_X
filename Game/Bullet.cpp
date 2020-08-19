#include "Bullet.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"

Bullet::Bullet(int id, float mass, float damage, float initSpeed): 
	Sprite(id),	m_mass(mass), m_damage(damage), m_initSpeed(initSpeed)
{
}
Bullet::Bullet(int id, Bullet& bullet)
	: Sprite(id), m_mass(bullet.m_mass), m_damage(bullet.m_damage), m_initSpeed(bullet.m_initSpeed)
{
	Init(bullet); //init for sprite
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime)
{
	
}
