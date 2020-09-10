#include "GS_TutorialState.h"
#include "../Singleton.h"
#include "../SceneManager2D.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "GameStateManager.h"

GS_TutorialState::GS_TutorialState()
{
	int iMaterialId;
	int iMainTexId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;

	position = Vector3(0, 6, 3);
	iMaterialId = 0;
	iMainTexId = 5;
	rotation = 0.0;
	scale = Vector2(3.5, 4.2);
	uiHexColor = 0xFFFFFF;
	alpha = 0.8;
	this->background = new Sprite(-1);
	this->background->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);

	position = Vector3(0, 0, -1);
	iMaterialId = 0;
	iMainTexId = 7;
	rotation = 0.0;
	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xc22e53;
	alpha = 1;
	UIComponent* intro = new UIComponent(-1);
	intro->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	intro->SetBound(0.6, -0.5, -0.7, 0.6);
	intro->SetRenderType(UIComponent::FitWidth);
	//intro->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	data.push_back(intro);

	position = Vector3(0, 0, -1);
	iMaterialId = 0;
	iMainTexId = 6;
	rotation = 0.0;
	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xc22e53;
	alpha = 1;
	UIComponent* control = new UIComponent(-1);
	control->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	control->SetBound(0.6, -0.5, -0.7, 0.6);
	control->SetRenderType(UIComponent::FitWidth);
	//control->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	data.push_back(control);

	this->current = 0;

	position = Vector3(0, 0, -1);
	iMaterialId = 1;
	iMainTexId = 5;
	rotation = 0.0;
	scale = Vector2(2, 2);
	uiHexColor = 0xc22e53;
	alpha = 1;
	int iFontID = 1;
	text = new UIText(-1);
	text->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	text->SetFont(iFontID);
	text->SetBound(-0.9, -0.95, -0.1, -0.05);
	text->SetText("1/2");
}

GS_TutorialState::~GS_TutorialState()
{
	delete background;
	for (int i = 0; i < data.size(); i++) {
		delete data[i];
	}
	std::vector<UIComponent*>().swap(data);
	
	delete text;
}

bool GS_TutorialState::Create()
{
	return false;
}

bool GS_TutorialState::Release()
{
	return false;
}

void GS_TutorialState::Render()
{
	this->background->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->data[current]->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
	this->text->Render(&Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MENU_OBJECT));
}

void GS_TutorialState::Update(float deltaTime)
{
}

void GS_TutorialState::KeyPress()
{
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::A) && current == 1) {
		current--;
		char Text[20];
		snprintf(Text, sizeof(Text), "%d/2", (current + 1));
		text->SetText(Text);
	}
	else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) && current == 0) {
		current++;
		char Text[20];
		snprintf(Text, sizeof(Text), "%d/2", (current + 1));
		text->SetText(Text);
	}


	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::ESCAPE)) {
		Singleton<GameStateManager>::GetInstance()->Pop();
	}
}
