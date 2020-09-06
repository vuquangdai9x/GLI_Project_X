#include "MapSection.h"

#include "Obstacle.h"

MapSection::MapSection() {
	m_id = -1;
	m_difficulty = 0;
	m_bottom = m_top = 0;
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

void MapSection::GenerateVoid(int id, float bottom, float top, float left, float right)
{
	m_id = id;
	m_bottom = bottom;
	m_top = top;
	m_left = left;
	m_right = right;
	printf("Generate Void Section %d: bottom = %f, top = %f, left = %f, right = %f\n", id, bottom, top, left, right);
}

void MapSection::Generate(int id, float difficulty, float bottom, float top, float left, float right, int iNumOfRoute, int iThemeId, int iThemeId2)
{
	m_id = id;
	m_difficulty = difficulty;
	m_bottom = bottom;
	m_top = top;
	m_left = left;
	m_right = right;
	printf("Generate Section %d: difficulty = %f | bottom = %f, top = %f, left = %f, right = %f | numOfRoutes: %d\n", id, difficulty, bottom, top, left, right,iNumOfRoute);
	
	float routeLeft = left;
	float routeLength = (right - left) / iNumOfRoute;
	
	for (int i = 0;i < iNumOfRoute;i++) {
		// TODO: generate map element for each route
		
		// get random map element from AccessLibrary
		// call generate() of this random element
		unsigned int color = rand() % 0xFFFFFF;
		Obstacle * obs = new Obstacle(0, 0);
		obs->Init(Vector3((routeLeft+routeLength/2), (top+bottom)/2, 14), 0, Vector2(1, 1), color, 1.0, 0, 20501);
		obs->createTriangle2D();
		m_listObject.push_back(obs);

		routeLeft += routeLength;
	}
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

float MapSection::GetBottom()
{
	return m_bottom;
}

float MapSection::GetTop()
{
	return m_top;
}

void MapSection::AddObject(Sprite* obj)
{
	m_listObject.push_back(obj);
}
