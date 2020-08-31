#include"MapEditor.h"
#include "GS_PauseState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../Obstacle.h"
#include "../SuicideBug.h"
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

	int checkChoose = Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT);
	if (m_currentSprite!=NULL&& (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_MOVE|| Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_RELEASE)&& !checkChoose) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		Vector3 Pos = m_currentSprite->GetPosition();
		Pos.x = pos3D.x;
		Pos.y = pos3D.y;
		m_currentSprite->SetPosition(Pos);
		for (int i = 0;i < m_activeList.size();i++) {
			Vector3 tmpPos= m_activeList[i]->GetPosition();
			tmpPos.x = Pos.x + m_distance_x[i];
			tmpPos.y = Pos.y + m_distance_y[i];
			m_activeList[i]->SetPosition(tmpPos);
		}
		if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE)) {
			deleteSprite(m_currentSprite);
			if (!m_activeList.empty()) {
				for (int i = 0;i < m_activeList.size();i++) {
					deleteSprite(m_activeList[i]);
				}
			}
			m_activeList.clear();
			m_distance_x.clear();
			m_distance_y.clear();
			m_currentSprite = NULL;
		}
		else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::DOWN)) {
			Sprite* tmp=cloneSprite( m_currentSprite);
			m_currentSprite->SetColor(m_norColor, 1);
			m_currentSprite = tmp;
			Singleton<SceneManager2D>::GetInstance()->AddObject(tmp, MAP_OBJECT);
			pushSprite(tmp);
			if (!m_activeList.empty()) {
				std::vector<Sprite*> v_tmp;
				for (int i = 0;i < m_activeList.size();i++) {
					Sprite* tmp=cloneSprite( m_activeList[i]);
					m_activeList[i]->SetColor(m_norColor, 1);;
					Singleton<SceneManager2D>::GetInstance()->AddObject(tmp, MAP_OBJECT);
					pushSprite(tmp);
					v_tmp.push_back(tmp);
				}
				m_activeList.clear();
				for (int i = 0;i < v_tmp.size();i++) {
					m_activeList.push_back(v_tmp[i]);
				}
			}
			Singleton<InputManager>::GetInstance()->KeyPressed(VK_DOWN, false);
			

		}
		//Singleton<InputManager>::GetInstance()->fixButton();
		
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK&& m_currentSprite==NULL) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		m_currentSprite = checkInside(pos3D.x, pos3D.y);
		if(m_currentSprite) m_currentSprite->SetColor(m_chooseColor,1);
		Singleton<InputManager>::GetInstance()->fixButton();
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK&& m_currentSprite != NULL&&!checkChoose) {
		m_currentSprite->SetColor(m_norColor, 1);
		m_currentSprite = NULL; Singleton<InputManager>::GetInstance()->fixButton();
		
		for (int i = 0;i < m_activeList.size();i++) m_activeList[i]->SetColor(m_norColor, 1);
		m_distance_x.clear();m_distance_y.clear();
		m_activeList.clear();
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK && m_currentSprite != NULL && checkChoose) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		Sprite* chooseSprite = checkInside(pos3D.x, pos3D.y),*tmpSprite;
		
		if (chooseSprite) {	
			m_activeList.push_back(m_currentSprite);
			m_currentSprite = chooseSprite;
			m_currentSprite->SetColor(m_chooseColor, 1);
			Vector2 distace(chooseSprite->GetPosition().x - m_currentSprite->GetPosition().x, chooseSprite->GetPosition().y - m_currentSprite->GetPosition().y);
			m_distance_x.push_back(distace.x);m_distance_y.push_back(distace.y);
			for (int i = 0;i < m_activeList.size() ;i++) {
				Vector2 distace(m_activeList[i]->GetPosition().x - m_currentSprite->GetPosition().x, m_activeList[i]->GetPosition().y - m_currentSprite->GetPosition().y);
				m_distance_x[i] = distace.x;
				m_distance_y[i] = distace.y;
			}
		}
		Singleton<InputManager>::GetInstance()->fixButton();
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
	if (m_currentSprite) m_currentSprite->SetColor(m_norColor, 1);
	if (!m_activeList.empty()) {
		for (int i = 0;i < m_activeList.size();i++) {
			m_activeList[i]->SetColor(m_norColor, 1);
		}
		m_activeList.clear();
		m_distance_x.clear();
		m_distance_y.clear();
	}
	
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
		SuicideBug* obs = new SuicideBug(m_curent->id);
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
		if (m_currentSprite) m_currentSprite->SetColor(m_norColor, 1);
		if (!m_activeList.empty()) {
			for (int i = 0;i < m_activeList.size();i++) {
				m_activeList[i]->SetColor(m_norColor, 1);
			}
			m_activeList.clear();
			m_distance_x.clear();
			m_distance_y.clear();
		}

		m_currentSprite = obs;
		Singleton<SceneManager2D>::GetInstance()->AddObject(obs, MAP_OBJECT);

		m_suicudeBug.push_back(obs);
	}

}

void MapEditor::KeyPress()
{
}

void MapEditor::deleteSprite(Sprite* pointer)
{
	for (int i = 0;i < m_Obstacle.size();i++) {
		if (m_Obstacle[i] == pointer) {
			m_Obstacle.erase(m_Obstacle.begin() + i);
			break;
		}
	}
	for (int i = 0;i < m_suicudeBug.size();i++) {
		if (m_suicudeBug[i] == pointer) {
			m_suicudeBug.erase(m_suicudeBug.begin() + i);
			break;
		}
	}
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(pointer);
	delete pointer;
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

void MapEditor::pushSprite(Sprite* pointer)
{
	if (dynamic_cast<Obstacle*>(pointer)) {
		m_Obstacle.push_back(pointer);
	}else if (dynamic_cast<SuicideBug*>(pointer)) {
		m_suicudeBug.push_back(pointer);
	}
}

Sprite* MapEditor::cloneSprite( Sprite* source)
{
	Sprite* destination=NULL;
	if (dynamic_cast<Obstacle*>(source)) {
		destination = new Obstacle(0, 0);
		destination->Init(*source);
	}
	else if (dynamic_cast<SuicideBug*>(source)) {
		destination = new SuicideBug(0);
		destination->Init(*source);
	}
	return destination;
}

