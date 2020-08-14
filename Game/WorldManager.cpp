#include "WorldManager.h"

WorldManager::WorldManager()
{
	
	b2Vec2 gravity(0.0f, -GRAVITY);
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -GRAVITY);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

}

WorldManager::~WorldManager()
{
	delete m_world;
}

ItemBody* WorldManager::createRectagle(int type, float x, float y, float w, float h,float massD)
{
	ItemBody *tmp=new ItemBody(type, x, y);
	b2BodyDef bodyDef;
	if (type != OBSTACLE) {
		bodyDef.type = b2_dynamicBody;
	}
	else {
		bodyDef.type = b2_staticBody;
	}
	
	bodyDef.position.Set(x, y);
	tmp->body= m_world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;
	tmp->body->CreateFixture(&fixtureDef);

	b2MassData mass;
	mass.center = tmp->body->GetWorldCenter();
	mass.mass = massD;
	tmp->body->SetMassData(&mass);
	listObject.push_back(tmp);
	return tmp;
}

void WorldManager::Update(float deltaTime)
{
	m_world->Step(deltaTime, m_velocityIterations, m_positionIterations);
	for (int i = 0;i < listObject.size();i++) {
		listObject[i]->Update(deltaTime);
	}
	
}
