#include "stdafx.h"
#include "SceneManager2D.h"
#include <math.h>
#include "../Framework3D/TrainingFramework/Globals.h"
#include "Sprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "WorldManager.h"
#include"Singleton.h"
#include"Button.h"
#include "State/GameStateManager.h"
#include "FloatingFish.h"
SceneManager2D::~SceneManager2D()
{
	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->OnDestroy();
		delete m_listObject[i];
	}
	delete m_mainCamera;
}

bool SceneManager2D::LoadScene(char* dataSceneFile) {
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataSceneFile);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("Fails to load scene file");
		return false;
	}

	Sprite* obj;
	int iNumOfObject, iObjectId = 0;
	int iMaterialId;
	int iMainTexId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	char shapeType[10];

	int iNumOfAnimations;
	int iAnimId;
	char animType[10];

	int iAnimTextureId;
	int iDivisionX, iDivisionY;
	float animDuration;

	int iNumOfAnimTexs;
	int* aiAnimTexId;
	int targetID;


	fscanf(fIn, "PLAYER %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	Player* player = new Player(iObjectId);
	player->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	player->createBox2D();
	AddObject(player);
	fscanf(fIn, "TARGETTEX %d\n", &targetID);
	fscanf(fIn, "TARGETSCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "TARGETPOS %f %f %f\n", &(position.x), &(position.y), &(position.z));
	Sprite* target = new Sprite(targetID);
	target->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, targetID);
	player->setTarget(target);
	AddObject(target);

	fscanf(fIn, "OBSTACLE_TYPE_0 %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		Obstacle* obs = new Obstacle(0);
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		obs->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		fscanf(fIn, "TYPE %s\n", shapeType);
		if (strcmp(shapeType, "RECT") == 0) {
			obs->createBox2D();
		}
		else {
			obs->createTriangle2D();
		}

		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		for (int i = 0;i < iNumOfAnimations;i++) {
			fscanf(fIn, "ANIM %d %s\n", &iAnimId, animType);
			if (strcmp("SPLIT", animType) == 0) {
				fscanf(fIn, "TEX %d\n", &iAnimTextureId);
				fscanf(fIn, "DIVIDE %d %d\n", &iDivisionX, &iDivisionY);
				fscanf(fIn, "DURATION %f\n", &animDuration);
			}
			else if (strcmp("FILES", animType) == 0) {
				fscanf(fIn, "TEXTURES %d\n", &iNumOfAnimTexs);
				aiAnimTexId = new int[iNumOfAnimTexs];
				for (int j = 0;j < iNumOfAnimTexs;j++) {
					fscanf(fIn, "TEX %d\n", &(aiAnimTexId[j]));
				}
				fscanf(fIn, "DURATION %f\n", &animDuration);
				delete[] aiAnimTexId;
			}
			else {
				printf("[ERR] SceneManager: Animation type is invalid: %s\n", animType);
			}
		}

		AddObject(obs);

		printf("[msg] SceneManager: Loaded Sprite %d\n\tMaterial: %d\n\tMain Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}
	int numOfTarget;
	b2Vec2 Target[10];

	fscanf(fIn, "\n#ENEMY\n");
	fscanf(fIn, "FLOATINGFISH %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "NUM OF TARGET %d\n", &numOfTarget);
		for (int j = 0; j < numOfTarget; j++) {
			fscanf(fIn, "TARGET %f %f\n", &(Target[j].x), &(Target[j].y));
		}
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		FloatingFish* fish = new FloatingFish(numOfTarget,Target);
		fish->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		fish->createBox2D();
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		AddObject(fish);
	}
	float nearPlane, farPlane, zoom;

	fscanf(fIn, "\n#CAMERA\n");
	fscanf(fIn, "NEAR %f\n", &nearPlane);
	fscanf(fIn, "FAR %f\n", &farPlane);
	fscanf(fIn, "ZOOM %f\n", &zoom);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "DUTCH %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;

	Camera2D* camera = new Camera2D();
	camera->Init(position, rotation);

	float aspectRatio = Globals::screenWidth / (float)Globals::screenHeight;
	camera->SetOrthorgraphic(zoom, aspectRatio, nearPlane, farPlane);
	SetMainCamera(camera);
	printf("[msg] SceneManager: Set up Camera2D\n");

	obj = NULL;
	camera = NULL;

	return true;
}

void goToPlay() {
	Singleton<GameStateManager>::GetInstance()->Push(GameStateManager::PLAY);
}
void test1() {
	printf("2 \n");
}
bool SceneManager2D::LoadMenuScene(char* dataSceneFile)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataSceneFile);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("Fails to load scene file");
		return false;
	}

	int iNumOfObject, iObjectId = 0;
	int iMaterialId;
	int iMainTexId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	int iNumOfAnimations;

	fscanf(fIn, "BACKGROUND %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	Sprite* backGround = new Sprite();
	backGround->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(backGround, MENU_OBJECT);

	fscanf(fIn, "GAMENAME %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	Sprite* name = new Sprite();
	name->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(name, MENU_OBJECT);

	fscanf(fIn, "BUTTON %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		Button* button = new Button();
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		button->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		button->UpdateMember();
		if(i==0)
		button->OnClick(goToPlay);
		else button->OnClick(test1);
		AddObject(button, MENU_OBJECT);
	}

	float nearPlane, farPlane, zoom;

	fscanf(fIn, "#CAMERA\n");
	fscanf(fIn, "NEAR %f\n", &nearPlane);
	fscanf(fIn, "FAR %f\n", &farPlane);
	fscanf(fIn, "ZOOM %f\n", &zoom);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "DUTCH %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;

	Camera2D* camera = new Camera2D();
	camera->Init(position, rotation);

	float aspectRatio = Globals::screenWidth / (float)Globals::screenHeight;
	camera->SetOrthorgraphic(zoom, aspectRatio, nearPlane, farPlane);
	SetMainCamera(camera, MENU_OBJECT);
	printf("[msg] SceneManager: Set up Camera2D\n");

	return true;
}

void SceneManager2D::SetMainCamera(Camera2D* camera, int listObjet)
{
	if (listObjet == PLAY_OBJECT) {
		m_mainCamera = camera;
	}
	else {
		m_menuCamera = camera;
	}
}

Camera2D& SceneManager2D::GetMainCamera(int listObjet)
{
	if(listObjet==PLAY_OBJECT)
	return *m_mainCamera;
	return *m_menuCamera;
}

void SceneManager2D::Update(float frameTime, int listObjet) {
	if (listObjet == PLAY_OBJECT) {
		m_time += frameTime;
		Singleton<WorldManager>::GetInstance()->Update(frameTime);
		for (int i = 0; i < m_listObject.size(); i++) {
			m_listObject[i]->Update(frameTime);
		}
		m_mainCamera->Update(frameTime);
	}
	else {
		for (int i = 0; i < m_menuObject.size(); i++) {
			m_menuObject[i]->Update(frameTime);
		}
		m_menuCamera->Update(frameTime);
	}
}
void SceneManager2D::Render(int listObjet) {
	if (listObjet == PLAY_OBJECT) {
		for (int i = 0; i < m_listObject.size(); i++) {
			m_listObject[i]->Render(m_mainCamera);
		}
	}
	else {
		for (int i = 0; i < m_menuObject.size(); i++) {
			m_menuObject[i]->Render(m_menuCamera);
		}
	}
}
void SceneManager2D::AddObject(Sprite* object,int listObject) {
	if (listObject == PLAY_OBJECT) {
		if (m_listObject.size() == 0) {
			m_listObject.push_back(object);
			return;
		}
		float zPos = object->GetPosition().z;
		if (zPos >= m_listObject[0]->GetPosition().z) {
			m_listObject.insert(m_listObject.begin(), object);
			return;
		}
		for (int i = 1;i < m_listObject.size();i++) {
			if (m_listObject[i - 1]->GetPosition().z >= zPos && zPos >= m_listObject[i]->GetPosition().z) {
				m_listObject.insert(m_listObject.begin() + i, object);
				return;
			}
		}
		m_listObject.push_back(object);
	}
	else {
		if (m_menuObject.size() == 0) {
			m_menuObject.push_back(object);
			return;
		}
		float zPos = object->GetPosition().z;
		if (zPos >= m_menuObject[0]->GetPosition().z) {
			m_menuObject.insert(m_menuObject.begin(), object);
			return;
		}
		for (int i = 1;i < m_menuObject.size();i++) {
			if (m_menuObject[i - 1]->GetPosition().z >= zPos && zPos >= m_menuObject[i]->GetPosition().z) {
				m_menuObject.insert(m_menuObject.begin() + i, object);
				return;
			}
		}
		m_menuObject.push_back(object);
	}
}
Sprite& SceneManager2D::GetObjectByID(int id)
{
	for (int i = 0; i < m_listObject.size(); i++) {
		if (m_listObject[i]->GetId() == id)
			return *(m_listObject[i]);
	}
}

std::vector<Sprite*>& SceneManager2D::GetListObject() {
	return m_listObject;
}

Vector2& SceneManager2D::get2Dpos(float x, float y, float z, int listObjet)
{
	float Dim[3];
	int viewPort[4];
	viewPort[0] = 0;viewPort[1] = 0;
	viewPort[2] = Globals::screenWidth;
	viewPort[3] = Globals::screenHeight;
	if (listObjet == PLAY_OBJECT)
	glhProjectf(x, y, z, m_mainCamera->GetViewMatrix(), m_mainCamera->GetProjectionMatrix(), viewPort, Dim);
	else glhProjectf(x, y, z, m_menuCamera->GetViewMatrix(), m_menuCamera->GetProjectionMatrix(), viewPort, Dim);
	return2D.x = Dim[0];
	return2D.y = Dim[1];
	return return2D;
}

Vector3& SceneManager2D::get3Dpos(float x, float y, int listObjet)
{
	float Dim[3];
	int viewPort[4];
	viewPort[0] = 0;viewPort[1] = 0;
	viewPort[2] = Globals::screenWidth;
	viewPort[3] = Globals::screenHeight;
	if(listObjet==PLAY_OBJECT)
	glhUnProjectf(x, y, 0.0, m_mainCamera->GetViewMatrix(), m_mainCamera->GetProjectionMatrix(), viewPort, Dim);
	else glhUnProjectf(x, y, 0.0, m_menuCamera->GetViewMatrix(), m_menuCamera->GetProjectionMatrix(), viewPort, Dim);
	return3D.x = Dim[0];
	return3D.y = Dim[1];
	return3D.z = Dim[2];
	return return3D;
}
