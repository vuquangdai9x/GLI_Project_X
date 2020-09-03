#include "MapSection.h"

#include "Obstacle.h"

MapSection::MapSection() {
	m_id = -1;
	m_difficulty = 0;
	m_startHeight = m_endHeight = 0;
}

MapSection::~MapSection() {
}

void MapSection::Clear()
{
	for (int i = 0;i < m_listObject.size(); i++) {
		delete m_listObject[i];
	}
	m_listObject.clear();
	printf("Clear Section %d\n", m_id);
}

void MapSection::Generate(int id, float difficulty, float startHeight, float endHeight)
{
	m_id = id;
	m_difficulty = difficulty;
	m_startHeight = startHeight;
	m_endHeight = endHeight;
	printf("Generate Section %d: difficulty = %f | startH = %f, endH = %f\n", id, difficulty, startHeight, endHeight);
	
	float x = ((float)rand() / (float)RAND_MAX * 2 - 1) * 20;
	unsigned int color = rand() % 0xFFFFFF;
	Obstacle* obs = new Obstacle(0, 0);
	obs->Init(Vector3(x, startHeight, 14), 0, Vector2(1, 1), color, 1.0, 0, 20501);
	obs->createTriangle2D();
	m_listObject.push_back(obs);
	obs = new Obstacle(0, 0);
	obs->Init(Vector3(x, endHeight, 14), 0, Vector2(1, 1), color, 1.0, 0, 20401);
	obs->createTriangle2D();
	m_listObject.push_back(obs);
}

void MapSection::Update(float deltaTime)
{
	for (int i = 0;i < m_listObject.size(); i++) {
		m_listObject[i]->Update(deltaTime);
	}
}

void MapSection::Render(Camera2D* camera)
{
	for (int i = 0;i < m_listObject.size(); i++) {
		m_listObject[i]->Render(camera);
	}
}

float MapSection::GetStartHeight()
{
	return m_startHeight;
}

float MapSection::GetEndHeight()
{
	return m_endHeight;
}

void MapSection::AddObject(Sprite* obj)
{
	m_listObject.push_back(obj);
	float newHeight = obj->GetPosition().y + obj->GetOriginSize().y / 2;
	if (m_endHeight < newHeight) {
		m_endHeight = newHeight;
	}
}
