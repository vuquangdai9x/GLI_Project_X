#include "WorldManager.h"
#include "ListenerClass.h"
#include "UserData.h"

uint16 WorldManager::GetMaskBits(int type) {
	uint16 mask = 0;
	switch (type)
	{
	case PLAYER:
		mask = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET) | (1 << SPECIAL_ENEMY) | (1 << MAP_BORDER);
		break;
	case ENEMY:
		mask = (1 << PLAYER) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << PLAYERBULLET);
		break;
	case SPECIAL_ENEMY:
		mask = (1 << PLAYERBULLET) | (1 << PLAYER);
		break;
	case OBSTACLE:
		mask = (1 << PLAYER) | (1 << OBSTACLE) | (1 << ENEMY) | (1 << PLAYERBULLET) | (1 << ENEMYBULLET) | (1 << ITEM);
		break;
	case PLAYERBULLET:
		mask = (1 << DECORATE_OBJ) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << ENEMYBULLET) | (1 << SPECIAL_ENEMY) | (1 << MAP_BORDER) | (1 << ITEM);
		break;
	case ENEMYBULLET:
		mask = (1 << DECORATE_OBJ) | (1 << PLAYER) | (1 << OBSTACLE) | (1 << PLAYERBULLET) | (1 << MAP_BORDER);
		break;
	case MAP_BORDER:
		mask = (1 << PLAYER) | (1 << PLAYERBULLET) | (1 << ENEMYBULLET);
		break;
	case ITEM:
		mask = (1 << PLAYER) | (1 << PLAYERBULLET) | (1 << ENEMY) | (1 << OBSTACLE) | (1 << SPECIAL_ENEMY) | (1 << MAP_BORDER);
		break;
	case DECORATE_OBJ:
		mask = (1 << PLAYERBULLET) | (1 << ENEMYBULLET);
		break;
	}
	return mask;
}

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
	if (type == OBSTACLE || type == DECORATE_OBJ) {
		bodyDef.type = b2_staticBody;
	}
	else if (type == MAP_BORDER) {
		bodyDef.type = b2_kinematicBody;
	}
	else {
		bodyDef.type = b2_dynamicBody;
	}
	
	bodyDef.position.Set(x, y);
	tmp->body= m_world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;
	if (type == SPECIAL_ENEMY || type == DECORATE_OBJ) fixtureDef.isSensor = true;
	// set filter
	fixtureDef.filter.categoryBits = (short)(1 << type);
	fixtureDef.filter.maskBits = GetMaskBits(type);

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
	if (type == OBSTACLE || type == DECORATE_OBJ) {
		bodyDef.type = b2_staticBody;
	}
	else if (type == MAP_BORDER) {
		bodyDef.type = b2_kinematicBody;
	}
	else {
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(x, y);
	tmp->body = m_world->CreateBody(&bodyDef);
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
	if (type == SPECIAL_ENEMY || type == DECORATE_OBJ) fixtureDef.isSensor = true;
	// set filter
	fixtureDef.filter.categoryBits = (short)(1 << type);
	fixtureDef.filter.maskBits = GetMaskBits(type);

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
ItemBody* WorldManager::createFloating(int type, float x, float y, float w, float h, float massD)
{
	ItemBody* tmp = new ItemBody(type, x, y);
	b2BodyDef bodyDef;
	if (type == OBSTACLE || type == DECORATE_OBJ) {
		bodyDef.type = b2_staticBody;
	}
	else if (type == MAP_BORDER) {
		bodyDef.type = b2_kinematicBody;
	}
	else {
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(x, y);
	tmp->body = m_world->CreateBody(&bodyDef);
	b2PolygonShape dynamicPolygon;
	b2Vec2 vertices[3];
	vertices[0].Set(0, h);
	vertices[1].Set(-w, -h / 2);
	vertices[2].Set(w, -h / 2);
	dynamicPolygon.Set(vertices, 3);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicPolygon;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.0f;
	if (type == SPECIAL_ENEMY || type == DECORATE_OBJ) fixtureDef.isSensor = true;
	// set filter
	fixtureDef.filter.categoryBits = (short)(1 << type);
	fixtureDef.filter.maskBits = GetMaskBits(type);

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

void WorldManager::CleanUp()
{
	for (int i = 0; i < listObject.size(); i++) {
		listObject[i]->body->GetWorld()->DestroyBody(listObject[i]->body);
	}
	std::vector<ItemBody*> empty;
	empty.swap(listObject);
	
}
 