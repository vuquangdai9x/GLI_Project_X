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
	
	if (m_curent != NULL) {
		addObject();
		m_curent = NULL;



	}
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->Update(deltaTime);
	}
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MAP_OBJECT);
	Vector3 cameraPos = camera.GetPosition();
	cameraPos.y += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S)) * deltaTime*camera.GetZoom();
	cameraPos.x += ((Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A))) * deltaTime* camera.GetZoom();
	camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Q), deltaTime);
	camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Z), deltaTime);
	camera.SetPosition(cameraPos);
	if (m_currentSprite!=NULL&& Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_MOVE) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		Vector3 Pos = m_currentSprite->GetPosition();
		Pos.x = pos3D.x;
		Pos.y = pos3D.y;
		m_currentSprite->SetPosition(Pos);
		if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE)) {
			for (int i = 0;i < m_Obstacle.size();i++) {
				if (m_Obstacle[i] == m_currentSprite) {
					m_Obstacle.erase(m_Obstacle.begin() + i);
					break;
				}
			}
			for (int i = 0;i < m_suicudeBug.size();i++) {
				if (m_suicudeBug[i] == m_currentSprite) {
					m_suicudeBug.erase(m_suicudeBug.begin() + i);
					break;
				}
			}
			Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_currentSprite);
			delete m_currentSprite;
			m_currentSprite = NULL;
		}
		//Singleton<InputManager>::GetInstance()->fixButton();
		
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK&& m_currentSprite==NULL) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		m_currentSprite = checkInside(pos3D.x, pos3D.y);
		Singleton<InputManager>::GetInstance()->fixButton();
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK&& m_currentSprite != NULL) {
		m_currentSprite = NULL; Singleton<InputManager>::GetInstance()->fixButton();
	}
}

void MapEditor::addObject()
{
	if (m_curent->type == OBSTACLE_UNIT) {
	int iNumOfObject, iObjectId;
	int iMaterialId;
	int iMainTexId;
	int iFontId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	Obstacle* obs = new Obstacle(m_curent->id, 0);
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
	
		m_Obstacle.push_back(obs);
	}
	else if (m_curent->type == SUICIDEBUG_UNIT) {
		int iNumOfObject, iObjectId;
		int iMaterialId;
		int iMainTexId;
		int iFontId;
		Vector3 position;
		float rotation;
		Vector2 scale;
		unsigned int uiHexColor;
		float alpha;
		Obstacle* obs = new Obstacle(m_curent->id, 0);
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

		m_suicudeBug.push_back(obs);
	}

}

void MapEditor::KeyPress()
{
}

Sprite* MapEditor::checkInside(float x, float y)
{
	for (int i = 0;i < m_Obstacle.size();i++) {
		Vector3 pos3d = m_Obstacle[i]->GetPosition();
		float width = m_Obstacle[i]->GetOrgSize().x * m_Obstacle[i]->GetScale().x;
		float height = m_Obstacle[i]->GetOrgSize().y * m_Obstacle[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_Obstacle[i];
	}
	for (int i = 0;i < m_suicudeBug.size();i++) {
		Vector3 pos3d = m_suicudeBug[i]->GetPosition();
		float width = m_suicudeBug[i]->GetOrgSize().x * m_suicudeBug[i]->GetScale().x;
		float height = m_suicudeBug[i]->GetOrgSize().y * m_suicudeBug[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_suicudeBug[i];
	}
	return nullptr;
}

