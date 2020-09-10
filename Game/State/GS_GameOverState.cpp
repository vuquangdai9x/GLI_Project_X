#include "../stdafx.h"
#include "GS_GameOverState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../WorldManager.h"

void BackToMenu1()
{
	Singleton<GameStateManager>::GetInstance()->Pop();
}
void Reload1()
{
	Singleton<GameStateManager>::GetInstance()->Pop();

	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
GS_GameOverState::GS_GameOverState()
{
	printf("GS_PauseState");
	char sceneFile[50] = "Datas/scene2d-gameover.txt";
	loadGameOver(sceneFile);

	for (int i = 0; i < this->buttons.size(); i++) {
		buttons[i]->UpdateMember();
	}
	for (int i = 0; i < this->buttons.size(); i++) {
		if (i == 0) {
			buttons[i]->OnClick(BackToMenu1);
		}
		else if (i == 1) {
			buttons[i]->OnClick(Reload1);
		}
	}
}

GS_GameOverState::~GS_GameOverState()
{
	for (int i = 0; i < buttons.size(); i++) {
		delete buttons[i];
	}
	std::vector<Button*>().swap(buttons);
}

bool GS_GameOverState::Create()
{
	return false;
}

bool GS_GameOverState::Release()
{
	return false;
}

void GS_GameOverState::Render()
{
	this->background->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->gameover->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	for (int i = 0; i < this->buttons.size(); i++) {
		this->buttons[i]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	}
	this->score->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
}

void GS_GameOverState::Update(float deltaTime)
{ 
	for (int i = 0; i < this->buttons.size(); i++) {
		if (buttons[i]->Update()) {
			Singleton<InputManager>::GetInstance()->fixButton();
			printf("ok \n");
			return;
		}
	}
	//Singleton<InputManager>::GetInstance()->fixButton();
}

void GS_GameOverState::KeyPress()
{
}

void GS_GameOverState::loadGameOver(char* dataSceneFile)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataSceneFile);
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
	background->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);

	int iFontId;
	position = Vector3(0, 0, -1);
	fscanf(fIn, "GAMEOVER\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
	this->gameover = new UIText(-1);
	gameover->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	gameover->SetFont(iFontId);
	gameover->SetText("GAMEOVER");
	gameover->SetBound(top, bot, left, right);
	gameover->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	gameover->SetRenderType(UIComponent::RenderType::FitHeight);

	fscanf(fIn, "SCORE\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	fscanf(fIn, "BOUND %f %f %f %f\n", &top, &bot, &left, &right);
	this->score = new UIText(-1);
	score->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	score->SetFont(iFontId);
	char scoreText[20];
	int sc = Singleton<SceneManager2D>::GetInstance()->getPlayer()->getScore() + Singleton<SceneManager2D>::GetInstance()->getPlayer()->GetPosition().y;
	snprintf(scoreText, sizeof(scoreText), "Score: %d", sc);
	score->SetText(scoreText);
	score->SetBound(top, bot, left, right);
	score->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	score->SetRenderType(UIComponent::RenderType::FitHeight);

	fscanf(fIn, "BUTTON %d\n", &iNumOfObject);
	std::vector<Button*> listButton;

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

		buttons.push_back(button);
	}

}
