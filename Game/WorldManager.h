#pragma once
#include "Box2D/Box2D.h"
#include"ItemBody.h"
#include<vector>
class WorldManager
{
public:
	enum BodyType{Static, Kinematic, Dynamic};
	WorldManager();
	~WorldManager();
	ItemBody* createRectagle(int type, float x, float y, float w, float h, float massD = DEFAULT_MASS, BodyType bodyType = BodyType::Dynamic);
	ItemBody* createTriangle(int type, float x, float y, float w, float h, float massD = DEFAULT_MASS, BodyType bodyType = BodyType::Dynamic);
	ItemBody* createFloating(int type, float x, float y, float w, float h, float massD = DEFAULT_MASS, BodyType bodyType = BodyType::Dynamic);
	void Update(float deltaTime);
	void CleanUp();
private:
	uint16 GetMaskBits(int type);
	b2World* m_world;
	int m_numberOfObject = 0;
	int32 m_velocityIterations = 6;
	int32 m_positionIterations = 2;
	std::vector<ItemBody*> listObject;
};

