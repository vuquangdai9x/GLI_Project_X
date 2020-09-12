#include "GS_SelectLevel.h"
#include "../Singleton.h"
#include "../SceneManager2D.h"
#include "GameStateManager.h"
#include"../WorldManager.h"

void SelectLevel1() {
	Singleton<GameStateManager>::GetInstance()->Pop();
	//if (map == 2) return;
	//WorldManager *test = Singleton<WorldManager>::GetInstance();

	Singleton<GameStateManager>::GetInstance()->setMap(1);
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}

void SelectLevel2() {
	Singleton<GameStateManager>::GetInstance()->Pop();
	//if (map == 2) return;
	//WorldManager *test = Singleton<WorldManager>::GetInstance();

	Singleton<GameStateManager>::GetInstance()->setMap(2);
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}

void SelectLevel3() {
	Singleton<GameStateManager>::GetInstance()->Pop();
	//if (map == 2) return;
	//WorldManager *test = Singleton<WorldManager>::GetInstance();

	Singleton<GameStateManager>::GetInstance()->setMap(3);
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}

void SelectMapEdit() {
	Singleton<GameStateManager>::GetInstance()->Pop();
	//if (map == 2) return;
	//WorldManager *test = Singleton<WorldManager>::GetInstance();

	Singleton<GameStateManager>::GetInstance()->setMap(100);
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
GS_SelectLevel::GS_SelectLevel()
{
	char sceneFile[50] = "Datas/scene2d-selectlevel.txt";
	this->LoadScene(sceneFile);

	for (int i = 0; i < this->numberOfLevel; i++) {
		buttons[i]->UpdateMember();
	}
	for (int i = 0; i < this->numberOfLevel; i++) {
		if (i == 0) {
			buttons[i]->OnClick(SelectLevel1);
		}
		else if (i == 1) {
			buttons[i]->OnClick(SelectLevel2);
		}
		else if (i == 2) {
			buttons[i]->OnClick(SelectLevel3);
		}
	}
	this->bt->UpdateMember();
	this->bt->OnClick(SelectMapEdit);
}

GS_SelectLevel::~GS_SelectLevel()
{
	delete background;
	for (int i = 0; i < buttons.size(); i++) {
		delete buttons[i];
	}
	std::vector<Button*>().swap(buttons);
	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	std::vector<UIText*>().swap(levels);
	for (int i = 0; i < lock.size(); i++) {
		delete lock[i];
	}
	std::vector<UIComponent*>().swap(lock);
	delete level;
	delete bt;
	delete star;
}

bool GS_SelectLevel::Create()
{
	return false;
}

bool GS_SelectLevel::Release()
{
	return false;
}

void GS_SelectLevel::Render()
{
	this->background->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->level->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	for (int i = 0; i < this->buttons.size(); i++) {
		this->buttons[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
	for (int i = 0; i < this->levels.size(); i++) {
		this->levels[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
	for (int i = 0; i < this->lock.size(); i++) {
		this->lock[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
	this->bt->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->star->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	
}

void GS_SelectLevel::Update(float deltaTime)
{
	for (int i = 0; i < this->numberOfLevel; i++) {
		if (buttons[i]->Update()) {
			Singleton<InputManager>::GetInstance()->fixButton();
			return;
		}
	}
	if (this->bt->Update()) {
		Singleton<InputManager>::GetInstance()->fixButton();
	}
}

void GS_SelectLevel::KeyPress()
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::ESCAPE)) {
		Singleton<GameStateManager>::GetInstance()->Pop();
	}
}

void GS_SelectLevel::LoadScene(char * dataScene)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataScene);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("Fails to load scene file");
	}

	int iNumOfObject, iObjectId = 0;
	int iMaterialId;
	int iMainTexId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	float top, bot, left, right;

	fscanf(fIn, "BACKGROUND %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);

	this->background = new Sprite(iObjectId);
	this->background->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);

	int iFontId;
	position = Vector3(0, 0, -1);
	fscanf(fIn, "LEVELSELECTED\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
	this->level = new UIText(-1);
	this->level->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	this->level->SetFont(iFontId);
	this->level->SetText("SECLECT LEVEL");
	this->level->SetBound(top, bot, left, right);
	this->level->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	this->level->SetRenderType(UIComponent::RenderType::FitHeight);

	fscanf(fIn, "BUTTON %d\n", &iNumOfObject);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	int y = 0;
	int x = 0;
	for (int i = 1; i <= iNumOfObject; i++) {
		Button* button = new Button(iObjectId);
		button->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		button->SetBound(0.25 - y * 0.3, 0.15 - y * 0.3, -0.65 + x * 0.3, -0.55 + x * 0.3);
		x++;
		if (i % 5 == 0) {
			y++;
			x = 0;
		}
		button->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
		button->SetRenderType(UIComponent::RenderType::FitHeight);

		this->buttons.push_back(button);
	}

	x = y = 0;

	fscanf(fIn, "LEVEL %d\n", &numberOfLevel);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);

	for (int i = 1; i <= numberOfLevel; i++) {
		
		UIText* lv = new UIText(iObjectId);
		lv->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		lv->SetBound(0.25 - y * 0.3, 0.15 - y * 0.3, -0.65 + x * 0.3, -0.55 + x * 0.3);
		x++;
		if (i % 5 == 0) {
			y++;
			x = 0;
		}
		lv->SetFont(iFontId);
		char Text[20];
		snprintf(Text, sizeof(Text), "%d", i);
		lv->SetText(Text);
		lv->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
		lv->SetRenderType(UIComponent::RenderType::FitHeight);

		this->levels.push_back(lv);
	}

	fscanf(fIn, "LOCK %d\n", &iNumOfObject);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	for (int i = numberOfLevel; i < numberOfLevel + iNumOfObject; i++) {
		UIComponent* locked = new UIComponent(iObjectId);
		locked->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		locked->SetBound(this->buttons[i]->GetTop(), this->buttons[i]->GetBottom(), this->buttons[i]->GetLeft(), this->buttons[i]->GetRight());
		locked->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
		locked->SetRenderType(UIComponent::RenderType::FitHeight);

		this->lock.push_back(locked);
	}
	this->bt = new Button(-1);
	this->bt->Init(Vector3(0, 0, -1), 0, Vector2(2, 2), 0xffffff, 1, 0, 60501);
	bt->SetBound(-0.65, -0.75, -0.05, 0.05);
	bt->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	bt->SetRenderType(UIComponent::RenderType::FitHeight);

	this->star = new UIComponent(-1);
	this->star->Init(Vector3(0, 0, -1), 0, Vector2(1.5, 1.5), 0xfaf38e, 1, 0, 60401);
	star->SetBound(-0.65, -0.75, -0.05 , 0.05);
	star->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	star->SetRenderType(UIComponent::RenderType::FitHeight);
}
