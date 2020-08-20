#pragma once
#include "Sprite.h"
#include "ItemBody.h"
#include "Player.h"
class Bullet : public Sprite
{
protected:
	ItemBody* m_bulletBody;
	Player* m_player;
public:
	Bullet(int id);
	virtual ~Bullet();
	virtual void CreatePhysicsBody() = 0;
	virtual void Fire(Sprite* shooter, Vector2 startPos, Vector2 direction) = 0;
	virtual void Update(float deltaTime);
	void SetActiveBullet(bool value);
};

