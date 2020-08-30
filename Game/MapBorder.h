#pragma once
#include "SpriteLoopTexture.h"
#include "ItemBody.h"
#include "Player.h"
class MapBorder: public SpriteLoopTexture {
private:
	ItemBody* m_obstacleBody;
	Player* m_player;
	float m_damage;
public:
	MapBorder(int id, Player* player, float damage);
	~MapBorder();
	void Update(float deltaTime);
	void createCollider();
};