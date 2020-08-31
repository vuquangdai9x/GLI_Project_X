#pragma once
#include "Sprite.h"
#include "Item.h"
#include "ItemBody.h"
#include <vector>

class BrokenBalloon : public Sprite
{
private:
	float x, y;
	float width, height;

	int m_HP, m_maxHP;
	int m_damage;
	int m_type;
	int m_size;

	ItemBody* body;
	std::vector<Item*> items;
public:
	BrokenBalloon(int id, int type, int size);
	~BrokenBalloon();
	void Update(float deltaTime);
	void createBox2D();
	void initItem();

	const static int MILITARYBALLOON = 0;
	const static int MEDICBALLOON = 1;
	const static int SCIENCEBALLOON = 2;

	const static int SMALL = 1;
	const static int NORMAL = 2;
	const static int LARGE = 3;
};

