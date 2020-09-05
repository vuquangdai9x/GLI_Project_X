#include "Misson.h"
#include "Singleton.h"
#include "SceneManager2D.h"

Misson::Misson(int map)
{
	this->map = map;

	int iMaterialId;
	int iMainTexId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;

	position = Vector3(0, 0, -1);
	iMaterialId = 1;
	iMainTexId = 5;
	rotation = 0.0;
	scale = Vector2(2, 2);
	uiHexColor = 0xc22e53;
	alpha = 1;
	int iFontID = 3;

	UIText* title = new UIText(-1);
	title->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	title->SetFont(iFontID);
	title->SetBound(0.7, 0.5, -0.15, 0.15);
	title->SetText("MISSON");
	this->misson.push_back(title);

	scale = Vector2(0.5, 0.5);
	for (int i = 0; i < 3; i++) {
		UIText* text = new UIText(-1);
		text->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		text->SetFont(iFontID);
		text->SetBound(0.45 - i * 0.1, 0.3 - i * 0.1, -0.85, 0.85);
		text->SetAlignHorizontal(UIComponent::AlignHorizontal::Center);
		text->SetRenderType(UIComponent::FitHeight);
		this->misson.push_back(text);
	}
	
	char dataMisson[20];
	snprintf(dataMisson, sizeof(dataMisson), "Datas/misson%d.txt", map);

	this->m_numberOfStar = 0;
	this->loadMisson(dataMisson);
}

Misson::~Misson()
{
}

void Misson::Render()
{
	for (int i = 0; i < this->misson.size(); i++) {
		this->misson[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
}

void Misson::loadMisson(char* dataMisson)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataMisson);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("Fails to load scene file");
	}

	char missonName[256];

	for (int i = 0; i < 3; i++) {
		fscanf(fIn, "%[^\n]\n", missonName);
		this->misson[i+1]->SetText(missonName);
	}

	int iMissonID;
	this->m_listMisson = new int[3];
	for (int i = 0; i < 3; i++) {
		fscanf(fIn, "ID %d\n", &iMissonID);
		if (iMissonID == 1) {
			fscanf(fIn, "ENEMYTYPE %d\n",&this->m_EnemyType);
			fscanf(fIn, "KILL %d\n", &this->m_MaxKill);
			this->m_kill = 0;
		}
		else if (iMissonID == 3) {
			fscanf(fIn, "SCORE %d\n", &this->m_MaxScore);
		}
		else {
			this->m_Blood = true;
		}
		m_listMisson[i] = iMissonID;
	}
}

void Misson::countKill()
{
	m_kill++;
}

int Misson::countStar()
{
	this->m_numberOfStar++;
	for (int i = 0; i < 3; i++) {
		if (m_listMisson[i] == 1 && m_kill >= m_MaxKill) {
			this->m_numberOfStar++;
		}
		else if (m_listMisson[i] == 2 && m_Blood == true) {
			this->m_numberOfStar++;
		}
		else if (m_listMisson[i] == 3 && m_Score >= m_MaxScore) {
			this->m_numberOfStar++;
		}
	}
	return this->m_numberOfStar;
}
