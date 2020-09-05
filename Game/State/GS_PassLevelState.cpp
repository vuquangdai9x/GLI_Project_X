#include "GS_PassLevelState.h"
#include "../Singleton.h"
#include "../SceneManager2D.h"
#include "GameStateManager.h"

void BackToMenu2()
{
	Singleton<GameStateManager>::GetInstance()->Pop();
}
void Reload2()
{
	Singleton<GameStateManager>::GetInstance()->Pop();
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
GS_PassLevelState::GS_PassLevelState()
{
	char sceneFile[50] = "Datas/scene2d-passlevel.txt";
	this->LoadScene(sceneFile);

	for (int i = 0; i < this->buttons.size(); i++) {
		buttons[i]->UpdateMember();
	}
	for (int i = 0; i < this->buttons.size(); i++) {
		if (i == 0) {
			buttons[i]->OnClick(BackToMenu2);
		}
		else if (i == 2) {
			buttons[i]->OnClick(Reload2);
		}
	}
}

GS_PassLevelState::~GS_PassLevelState()
{
}

bool GS_PassLevelState::Create()
{
	return false;
}

bool GS_PassLevelState::Release()
{
	return false;
}

void GS_PassLevelState::Render()
{
	this->background->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->level->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	for (int i = 0; i < this->buttons.size(); i++) {
		this->buttons[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
	for (int i = 0; i < this->stars.size(); i++) {
		this->stars[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
}

void GS_PassLevelState::Update(float deltaTime)
{
	for (int i = 0; i < this->buttons.size(); i++) {
		buttons[i]->Update(deltaTime);
	}
	Singleton<InputManager>::GetInstance()->fixButton();
}

void GS_PassLevelState::KeyPress()
{
}

void GS_PassLevelState::LoadScene(char* dataScene)
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
	fscanf(fIn, "LEVELCOMPLETED\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
	this->level = new UIText(-1);
	this->level->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	this->level->SetFont(iFontId);
	this->level->SetText("LEVEL COMPLETED!");
	this->level->SetBound(top, bot, left, right);
	this->level->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	this->level->SetRenderType(UIComponent::RenderType::FitHeight);

	fscanf(fIn, "STAR %d\n", &iNumOfObject);

	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
		UIComponent* star = new UIComponent(iObjectId);
		star->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		star->SetBound(top, bot, left, right);
		star->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
		star->SetRenderType(UIComponent::RenderType::FitHeight);

		this->stars.push_back(star);
	}

	int numOfStar = Singleton<SceneManager2D>::GetInstance()->getPlayer()->getMisson()->countStar();
	for (int i = numOfStar; i < 3; i++) {
		this->stars[i]->SetColor(0xfff7ff, 1);
	}

	
	fscanf(fIn, "BUTTON %d\n", &iNumOfObject);

	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
		Button* button = new Button(iObjectId);
		button->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		button->SetBound(top, bot, left, right);
		button->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
		button->SetRenderType(UIComponent::RenderType::FitHeight);

	this->buttons.push_back(button);
	}
}
