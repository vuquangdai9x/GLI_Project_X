#include"MapEditor.h"
#include "GS_PauseState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../Obstacle.h"
void getInfor() {
	
}
MapEditor::MapEditor()
{
	printf("GS_PauseState");
	char sceneFile[50] = "Datas/scene2d-map.txt";
	char resourcesFile[60] = "Datas/resources2d.txt";

	//Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);
	m_curent = NULL;
	m_currentSprite = NULL;
	button = Singleton<SceneManager2D>::GetInstance()->LoadMapScene(sceneFile);
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->UpdateMember();
		button[i]->setBuffer(&m_curent);
	}
}

bool MapEditor::Create()
{
	return false;
}

bool MapEditor::Release()
{
	return false;
}

void MapEditor::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render(MAP_OBJECT);
}

void MapEditor::Update(float deltaTime)
{
	if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) m_currentSprite = NULL;
	if (m_curent != NULL) {
		addObject();
		m_curent = NULL;



	}
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->Update(deltaTime);
	}
	//Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MAP_OBJECT);
	
	if (m_currentSprite!=NULL) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		float f_iMousePosX = iMousePosX, f_iMousePosY = iMousePosY;

		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MENU_OBJECT);
		Vector3 Pos = m_currentSprite->GetPosition();
		Pos.x = pos3D.x;
		Pos.y = pos3D.y;
		m_currentSprite->SetPosition(Pos);
		Singleton<InputManager>::GetInstance()->fixButton();
		
	}
}

void MapEditor::addObject()
{
	int iNumOfObject, iObjectId;
	int iMaterialId;
	int iMainTexId;
	int iFontId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	Obstacle* obs = new Obstacle(1, 0);
	iMaterialId = 0;
	iMainTexId = m_curent->mainTex;
	rotation = 0;
	scale.x = 1;
	scale.y = 1;
	uiHexColor = 0xffffff;
	alpha = 0.9;
	position.x = 3;
	position.y = 3;
	position.z = -0.9;
	obs->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_currentSprite = obs;
	Singleton<SceneManager2D>::GetInstance()->AddObject(obs, MAP_OBJECT);

}

void MapEditor::KeyPress()
{
}

