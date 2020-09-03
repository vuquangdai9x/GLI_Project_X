#include "EndlessSectionManager.h"
#include "Obstacle.h"

EndlessSectionManager::EndlessSectionManager(Player* player, float mapWidth)
{
	m_iSectionIdCount = 0;
	m_player = player;
	m_mapWidth = mapWidth;
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

	// generate starting section, player at y=0, current section start from 0
	printf("Generate current section as starting section\n");

	// generate obstacles
	Obstacle* startingGround = new Obstacle(0, 0);
	startingGround->Init(Vector3(0,-1.5,14), 0, Vector2(1,1), 0xFFFFFF, 1.0, 0, 20101);
	m_listMapSection[m_iCurrSectionIndex]->AddObject(startingGround);
	Obstacle* obs = new Obstacle(0, 0);
	obs->Init(Vector3(0, 15, 14), 0, Vector2(1, 1), 0xFFFFFF, 1.0, 0, 20601);
	obs->createTriangle2D();
	m_listMapSection[m_iCurrSectionIndex]->AddObject(obs);

	m_currStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetStartHeight();
	m_currEndHeight = m_listMapSection[m_iCurrSectionIndex]->GetEndHeight();

	// generate tutorials
	// ...

	// generate next sections
	m_genStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetEndHeight();
	for (int i = m_iCurrSectionIndex+1; i < m_iCurrSectionIndex + m_iNextOffset; i++) {
		m_iSectionIdCount++;

		float sectionHeight = 20 + rand() % 9 - 4;

		m_listMapSection[i % m_listMapSection.size()]->Generate(
			m_iSectionIdCount,
			m_difficultyCurve(m_genStartHeight),
			m_genStartHeight,
			m_genStartHeight + sectionHeight
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
		m_currStartHeight = m_listMapSection[m_iCurrSectionIndex]->GetStartHeight();
		m_currEndHeight = m_listMapSection[m_iCurrSectionIndex]->GetEndHeight();
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
			m_genStartHeight + sectionHeight
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
