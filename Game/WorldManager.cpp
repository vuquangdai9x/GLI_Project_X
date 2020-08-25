#include "WorldManager.h"
#include "ListenerClass.h"
#include "UserData.h"
WorldManager::WorldManager()
{
	
	//b2Vec2 gravity(0.0f, -GRAVITY);
	b2Vec2 gravity(0.0f, 0.0f);
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
	
	m_world->SetContactListener(new ListenerClass());
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -GRAVITY);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

}

WorldManager::~WorldManager()
{
	for (int i = 0;i < listObject.size();i++) {
		delete listObject[i];
	}
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

	// set filter
	fixtureDef.filter.categoryBits = (short)(1 << type);
	switch (type)
	{
	case PLAYER:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET)|(1 << SPECIAL_ENEMY);
		break;
	case ENEMY:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << PLAYERBULLET);
		break;
	case SPECIAL_ENEMY:
		fixtureDef.filter.maskBits = (1 << PLAYERBULLET)| (1 << PLAYER);
		break;
	case OBSTACLE:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY)  | (1 << PLAYERBULLET) | (1 << ENEMYBULLET);
		break;
	case PLAYERBULLET:
		fixtureDef.filter.maskBits = (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET|1<<SPECIAL_ENEMY);
		break;
	case ENEMYBULLET:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << OBSTACLE) | (1 << PLAYERBULLET);
		break;
	}

	tmp->body->CreateFixture(&fixtureDef);

	b2MassData mass;
	mass.center = tmp->body->GetWorldCenter(); 
	mass.mass = massD;
	tmp->body->SetMassData(&mass);

	UserData *user = new UserData;
	user->m_type = tmp->GetType();
	tmp->body->SetUserData(user);

	listObject.push_back(tmp);
	return tmp;
}

ItemBody* WorldManager::createTriangle(int type, float x, float y, float w, float h, float massD)
{
	ItemBody* tmp = new ItemBody(type, x, y);
	b2BodyDef bodyDef;
	if (type != OBSTACLE) {
		bodyDef.type = b2_dynamicBody;
	}
	else {
		bodyDef.type = b2_staticBody;
	}

	bodyDef.position.Set(x, y);
	tmp->body = m_world->CreateBody(&bodyDef);
	//printf("%f %f %f %f \n", x, y, w, h);
	b2PolygonShape dynamicPolygon;
	b2Vec2 vertices[3];
	vertices[0].Set(0,- h);
	vertices[1].Set(w, h);
	vertices[2].Set(- w, h);
	dynamicPolygon.Set(vertices, 3);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicPolygon;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;

	// set filter
	fixtureDef.filter.categoryBits = (short)(1 << type);
	switch (type)
	{
	case PLAYER:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET);
		break;
	case ENEMY:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << PLAYERBULLET);
		break;
	case OBSTACLE:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << ENEMY) | (1 << PLAYERBULLET) | (1 << ENEMYBULLET);
		break;
	case PLAYERBULLET:
		fixtureDef.filter.maskBits = (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET);
		break;
	case ENEMYBULLET:
		fixtureDef.filter.maskBits = (1 << PLAYER) | (1 << OBSTACLE) | (1 << PLAYERBULLET);
		break;
	}

	tmp->body->CreateFixture(&fixtureDef);
	b2MassData mass;
	mass.center = tmp->body->GetWorldCenter();
	mass.mass = massD;
	tmp->body->SetMassData(&mass);

	UserData* user = new UserData;
	user->m_type = tmp->GetType();
	tmp->body->SetUserData(user);

	listObject.push_back(tmp);
	return tmp;
}

void WorldManager::Update(float deltaTime)
{
	
	for (int i = 0;i < listObject.size();i++) {
		listObject[i]->body->SetActive(listObject[i]->getActive());
		listObject[i]->Update(deltaTime);
	}
	m_world->Step(deltaTime, m_velocityIterations, m_positionIterations);
}
