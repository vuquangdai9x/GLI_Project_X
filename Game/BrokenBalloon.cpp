#include "BrokenBalloon.h"
#include "Singleton.h"
#include "SceneManager2D.h"
#include"WorldManager.h"
#include "ItemBuffSpeed.h"
#include "ItemBuffDamage.h"
#include "ItemHeal.h"
#include "ItemAmoGun.h"
#include "UserData.h"
#include "SoundManager.h"
BrokenBalloon::BrokenBalloon(int id,int type,int size):Sprite(id)
{
	int iMaterialId = 0;
	int iMainTexId;

	if (type == MILITARYBALLOON) {
		iMainTexId = 80102;
	}
	else if (type == MEDICBALLOON) {
		iMainTexId = 80101;
	}
	else {
		iMainTexId = 80103;
	}
	Vector3 playerPos;
	Singleton<SceneManager2D>::GetInstance()->getPlayerPos(playerPos);
	Vector3 position = Vector3(playerPos.x,playerPos.y + 30, 1);
	float rotation = 0;
	Vector2 scale;
	if (size == SMALL) {
		scale = Vector2(0.5, 0.5);
	}
	else if (size == NORMAL) {
		scale = Vector2(0.8, 0.8);
	}
	else {
		scale = Vector2(1.0, 1.0);
	}
	unsigned int uiHexColor = 0xfbafa2;
	float alpha = 1;

	this->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	this->createBox2D();

	this->m_type = type;
	this->m_size = size;
	this->initItem();

	this->m_HP = this->m_maxHP = 10;
	this->m_damage = 0;
}

BrokenBalloon::~BrokenBalloon()
{
}

void BrokenBalloon::Update(float deltaTime)
{
	Vector3 ballonPos = this->GetPosition();

	ballonPos.x = this->body->body->GetPosition().x;
	ballonPos.y = this->body->body->GetPosition().y;
	this->SetPosition(ballonPos);

	UserData* user = (UserData*)this->body->body->GetUserData();
	if (user->IsCollison > 0) {
		this->SetColor(0xffafff, 1);
		this->m_HP -= user->m_receiveDamage;

		if (this->m_HP <= 0) {
			for (int i = 0; i < items.size(); i++) {
				items[i]->Use(Singleton<SceneManager2D>::GetInstance()->getPlayer());
			}
			Singleton<SoundManager>::GetInstance()->Player(SoundManager::P_GETITEM);
			Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
			this->body->setActive(false);
			delete this;
		}
	}

	else {
		this->SetColor(0xffffff, 1);
	}
}

void BrokenBalloon::createBox2D()
{
	x = m_position.x;
	y = m_position.y;
	width = m_originSize.x * this->GetScale().x;
	height = m_originSize.y * this->GetScale().y;
	body = Singleton<WorldManager>::GetInstance()->createRectagle(ITEM, x, y, width, height, 200);
	body->SetGravityScale(0);
	UserData* user = (UserData*)this->body->body->GetUserData();
	user->m_damage = m_damage;
}

void BrokenBalloon::initItem()
{
	if (this->m_type == MILITARYBALLOON) {
		ItemAmoGun* amo = new ItemAmoGun(30 * m_size , 15 * m_size);
		ItemBuffDamage* buffdamage = new ItemBuffDamage(2 * m_size);
		
		items.push_back(amo);
		items.push_back(buffdamage);
		
	}
	else if (this->m_type == MEDICBALLOON) {
		ItemHeal* heal = new ItemHeal(15 * m_size);
		
	    items.push_back(heal);
		
	}
	else {
		ItemBuffSpeed* buffSpeed = new ItemBuffSpeed(1.0 * m_size, 2.0 * m_size);

		items.push_back(buffSpeed);
	}
}
