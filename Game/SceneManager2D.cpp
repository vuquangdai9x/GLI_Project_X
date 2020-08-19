#include "stdafx.h"
#include "SceneManager2D.h"
#include <math.h>
#include "../Framework3D/TrainingFramework/Globals.h"
#include "Sprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "WorldManager.h"

#include "SimpleGun.h"
#include "GunBulletPool.h"

SceneManager2D::~SceneManager2D()
{
	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->OnDestroy();
		delete m_listObject[i];
	}
	if (m_mainCamera != NULL) delete m_mainCamera;
	if (m_combatController != NULL) delete m_combatController;
	for (int i = 0; i < m_ListBulletPool.size(); i++) {
		delete m_ListBulletPool[i];
	}
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

	// Set up player
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

	// Set up weapon & bullet
	int iPoolId;
	int iPoolCapacity;
	// TODO: write code to read from file. The code below is just for testing
	m_combatController = new CombatController(player);
	// init bullet pool
		// define a template bullet, then create bullet pool based on this template
	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1.0;
	iMaterialId = 0;
	iMainTexId = 9; // red bullet
	iPoolCapacity = 50;
	GunBullet* templateGunBullet = new GunBullet(-1, 1.0, 0.0, 5.0, 15.0, 2);
	templateGunBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(0, iPoolCapacity, 10, templateGunBullet));

	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1.0;
	iMaterialId = 0;
	iMainTexId = 10; // black bullet
	iPoolCapacity = 20;
	GunBullet* templateCannonBullet = new GunBullet(-1, 3.0, 1.0, 7.0, 10.0, 3);
	templateCannonBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(1, iPoolCapacity, 10, templateCannonBullet));

	// add guns
	//		standard example
	SimpleGun* gun;
	int iBulletPoolId;
	iBulletPoolId = 0;
	gun = new SimpleGun(0, "Pistol", 0, 0, iBulletPoolId, 0.5, 0.5, 0.0, 0.05, 0.0, 1, 1);
	m_combatController->AddWeapon(gun);
	//		add more gun. Max is 9 type of guns
	gun = new SimpleGun(1, "AK", 0, 0, iBulletPoolId, 0.5, 0.1, 0.1, 0.05, 0.1, 1, 3);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(2, "Shotgun", 0, 0, iBulletPoolId, 1.0, 1.0, 0.0, 1.00, 0.0, 5, 1);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(3, "Sniper", 0, 0, iBulletPoolId, 1.0, 1.5, 0.0, 0.00, 0.0, 1, 1);
	m_combatController->AddWeapon(gun);
	iBulletPoolId = 1;
	gun = new SimpleGun(4, "Cannon", 0, 0, iBulletPoolId, 1.0, 0.5, 0.0, 0.00, 0.0, 1, 1);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(5, "SuperCannon", 0, 0, iBulletPoolId, 1.0, 0.5, 0.0, 0.00, 0.0, 3, 1);
	m_combatController->AddWeapon(gun);
	// give player some bullets when start game
	m_combatController->AddBullet(0, 50);
	m_combatController->AddBullet(1, 20);
	

	//
	// set up other object
	fscanf(fIn, "OBSTACLE_TYPE_0 %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		Obstacle* obs = new Obstacle(iObjectId,0);
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

void SceneManager2D::SetMainCamera(Camera2D* camera)
{
	m_mainCamera = camera;
}

Camera2D& SceneManager2D::GetMainCamera()
{
	return *m_mainCamera;
}

void SceneManager2D::Update(float frameTime) {
	m_time += frameTime;
	Singleton<WorldManager>::GetInstance()->Update(frameTime);
	for (int i = 0; i < m_listObject.size(); i++) {
		if (m_listObject[i]->CheckIsActiveSprite())
			m_listObject[i]->Update(frameTime);
	}
	m_mainCamera->Update(frameTime);
	m_combatController->Update(frameTime);
}
void SceneManager2D::Render() {
	for (int i = 0; i < m_listObject.size(); i++) {
		if (m_listObject[i]->CheckIsActiveSprite())
			m_listObject[i]->Render(m_mainCamera);
	}
}
void SceneManager2D::AddObject(Sprite* object) {
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
		if (m_listObject[i-1]->GetPosition().z >= zPos && zPos >= m_listObject[i]->GetPosition().z) {
			m_listObject.insert(m_listObject.begin() + i, object);
			return;
		}
	}
	m_listObject.push_back(object);
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

Vector2& SceneManager2D::get2Dpos(float x, float y, float z)
{
	float Dim[3];
	int viewPort[4];
	viewPort[0] = 0;viewPort[1] = 0;
	viewPort[2] = Globals::screenWidth;
	viewPort[3] = Globals::screenHeight;
	glhProjectf(x,y, z, m_mainCamera->GetViewMatrix(), m_mainCamera->GetProjectionMatrix(), viewPort, Dim);
	return2D.x = Dim[0];
	return2D.y = Dim[1];
	return return2D;
}

Vector3& SceneManager2D::get3Dpos(float x, float y)
{
	float Dim[3];
	int viewPort[4];
	viewPort[0] = 0;viewPort[1] = 0;
	viewPort[2] = Globals::screenWidth;
	viewPort[3] = Globals::screenHeight;
	glhUnProjectf(x, y,0.0, m_mainCamera->GetViewMatrix(), m_mainCamera->GetProjectionMatrix(), viewPort, Dim);
	return3D.x = Dim[0];
	return3D.y = Dim[1];
	return3D.z = Dim[2];
	return return3D;
}
