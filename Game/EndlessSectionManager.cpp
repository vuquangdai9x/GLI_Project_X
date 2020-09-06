#include "EndlessSectionManager.h"
#include "Obstacle.h"
#include "MapElementManager.h"

EndlessSectionManager::EndlessSectionManager(Player* player, float leftBound, float rightBound)
{
	m_iSectionIdCount = 0;
	m_player = player;
	m_leftBound = leftBound;
	m_rightBound = rightBound;
	Singleton<MapElementManager>::CreateInstance();
}

EndlessSectionManager::~EndlessSectionManager()
{
	for (int i = 0; i < m_listMapSection.size(); i++) {
		delete m_listMapSection[i];
	}
}

void EndlessSectionManager::Load(FILE* fIn)
{
	fscanf(fIn, "\n#ENDLESS\n");
	char difCurveName[50];
	fscanf(fIn, "DIFFICULTY CURVE %s\n", difCurveName);
	m_difficultyCurve = DifficultyCurve::GetFunctionPtr(difCurveName);
	printf("Start init endless map. Difficulty curve: %s\n",difCurveName);

	// init section
	fscanf(fIn, "SECTION OFFSET PREV %d\n", &m_iPrevOffset);
	fscanf(fIn, "SECTION OFFSET NEXT %d\n", &m_iNextOffset);
	for (int i = 0; i < m_iNextOffset + m_iPrevOffset + 1;i++) {
		m_listMapSection.push_back(new MapSection());
	}
	m_iCurrSectionIndex = 0;
	m_iNextSectionIndex = (m_iCurrSectionIndex + m_iNextOffset) % m_listMapSection.size();
	m_iPrevSectionIndex = (m_listMapSection.size() + m_iCurrSectionIndex - m_iPrevOffset) % m_listMapSection.size();

	fscanf(fIn, "TRIGGER DISTANCE DEL PREV %f\n", &m_triggerDelPrevOffset);
	fscanf(fIn, "TRIGGER DISTANCE GEN NEXT %f\n", &m_triggerGenNextOffset);

	// load map elements
	// ...
	Vector3 pos(0, 0, 5);
	Vector2 scale(1, 1);
	Obstacle* piece = new Obstacle(202000, OBSTACLE);
	piece->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20201);
	piece->CreatePhysicsBody(Obstacle::ObstacleType::Piece, 3);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(piece); // small piece

	piece = new Obstacle(203000, OBSTACLE);
	piece->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20301);
	piece->CreatePhysicsBody(Obstacle::ObstacleType::Piece, 5);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(piece); // large piece

	Obstacle* island = new Obstacle(204000, OBSTACLE);
	island->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20401);
	island->CreatePhysicsBody(Obstacle::ObstacleType::Island, 3);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(island); // tiny island

	island = new Obstacle(205000, OBSTACLE);
	island->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20501);
	island->CreatePhysicsBody(Obstacle::ObstacleType::Island, 3);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(island); // small island

	island = new Obstacle(206000, OBSTACLE);
	island->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20601);
	island->CreatePhysicsBody(Obstacle::ObstacleType::Island, 3);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(island); // medium island

	island = new Obstacle(207000, OBSTACLE);
	island->Init(pos, 0, scale, 0xFFFFFF, 1.0, 0, 20602);
	island->CreatePhysicsBody(Obstacle::ObstacleType::Cliff, 3);
	Singleton<MapElementManager>::GetInstance()->AddObjectType(island); // medium cliff


	// generate starting section, player at y=0, current section start from 0
	printf("Generate current section as starting section\n");
	
	// generate obstacles
	m_listMapSection[m_iCurrSectionIndex]->GenerateVoid(0, 0, 20, m_leftBound, m_rightBound);
	Obstacle* startingGround = new Obstacle(0, 0);
	startingGround->Init(Vector3(0,-1.5,14), 0, Vector2(1,1), 0xFFFFFF, 1.0, 0, 20101);
	m_listMapSection[m_iCurrSectionIndex]->AddObject(startingGround);
	Obstacle* obs = new Obstacle(0, 0);
	obs->Init(Vector3(0, 15, 14), 0, Vector2(1, 1), 0xFFFFFF, 1.0, 0, 20601);
	obs->createTriangle2D();
	m_listMapSection[m_iCurrSectionIndex]->AddObject(obs);

	m_currStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetBottom();
	m_currEndHeight = m_listMapSection[m_iCurrSectionIndex]->GetTop();

	// generate tutorials
	// ...

	// generate next sections
	m_genStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetTop();
	for (int i = m_iCurrSectionIndex+1; i < m_iCurrSectionIndex + m_iNextOffset; i++) {
		m_iSectionIdCount++;

		float sectionHeight = 20 + rand() % 9 - 4;

		m_listMapSection[i % m_listMapSection.size()]->Generate(
			m_iSectionIdCount,
			m_difficultyCurve(m_genStartHeight),
			m_genStartHeight,
			m_genStartHeight + sectionHeight,
			m_leftBound, 
			m_rightBound,
			rand() % 3+2,
			rand()%3
		);
		m_genStartHeight += sectionHeight;
	}

	
	m_isDelPrev = m_isGenNext = false;
}

void EndlessSectionManager::Update(float deltaTime)
{
	float playerHeight = m_player->GetPosition().y;
	if (playerHeight > m_currEndHeight) {
		m_iCurrSectionIndex = (m_iCurrSectionIndex + 1) % m_listMapSection.size();
		m_isDelPrev = m_isGenNext = false;
		m_currStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetBottom();
		m_currEndHeight = m_listMapSection[m_iCurrSectionIndex]->GetTop();
	}
	if (!m_isDelPrev && playerHeight > m_currStartHeight + m_triggerDelPrevOffset) {
		//printf("Clear secIndex = %d | ", m_iPrevSectionIndex);
		m_listMapSection[m_iPrevSectionIndex]->Clear();
		m_iPrevSectionIndex = (m_iPrevSectionIndex + 1) % m_listMapSection.size();
		m_isDelPrev = true;
	}
	if (!m_isGenNext && playerHeight > m_currEndHeight - m_triggerGenNextOffset) {
		//printf("Gen secIndex = %d | ", m_iNextSectionIndex);
		m_iSectionIdCount++;

		// TODO: calculate height of next section
		float sectionHeight = 20 + rand() % 9 - 4;
		// ___

		m_listMapSection[m_iNextSectionIndex]->Generate(
			m_iSectionIdCount,
			m_difficultyCurve(m_genStartHeight),
			m_genStartHeight,
			m_genStartHeight + sectionHeight,
			m_leftBound,
			m_rightBound,
			rand() % 3 + 2,
			rand() % 3
		);
		m_genStartHeight += sectionHeight;
		m_iNextSectionIndex = (m_iNextSectionIndex + 1) % m_listMapSection.size();
		m_isGenNext = true;
	}

	for (int i = 0; i < m_listMapSection.size(); i++) {
		m_listMapSection[i]->Update(deltaTime);
	}
}

void EndlessSectionManager::Render(Camera2D* camera)
{
	for (int i = 0; i < m_listMapSection.size(); i++) {
		m_listMapSection[i]->Render(camera);
	}
}
