#include "stdafx.h"
#include "SceneManager2D.h"
#include <math.h>
#include "../Framework3D/TrainingFramework/Globals.h"
#include "Sprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "WorldManager.h"

#include"Button.h"
#include "State/GameStateManager.h"
#include "FloatingFish.h"

#include "SimpleGun.h"
#include "AutoGun.h"
#include "GunBulletPool.h"

#include "UIComponent.h"

SceneManager2D::~SceneManager2D()
{
	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->OnDestroy();
		delete m_listObject[i];
	}
	for (int i = 0; i < m_listUIComponents.size(); i++) {
		m_listUIComponents[i]->OnDestroy();
		delete m_listUIComponents[i];
	}
	if (m_mainCamera != NULL) delete m_mainCamera;
	if (m_combatController != NULL) delete m_combatController;
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
	fscanf(fIn, "TARGETTEX %d\n", &targetID);
	fscanf(fIn, "TARGETSCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "TARGETPOS %f %f %f\n", &(position.x), &(position.y), &(position.z));
	Sprite* target = new Sprite(targetID);
	target->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, targetID);
	player->setTarget(target);
	AddObject(target);

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
	iPoolCapacity = 500;
	GunBullet* templateGunBullet = new GunBullet(-1, 1.0, 0.0, 5.0, 30.0, 2);
	templateGunBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(0, iPoolCapacity, 10, templateGunBullet));

	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1.0;
	iMaterialId = 0;
	iMainTexId = 10; // black bullet
	iPoolCapacity = 200;
	GunBullet* templateCannonBullet = new GunBullet(-1, 3.0, 1.0, 7.0, 15.0, 3);
	templateCannonBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(1, iPoolCapacity, 10, templateCannonBullet));

	// add guns
	//		standard example
	Weapon* gun;
	int iBulletPoolId;
	iBulletPoolId = 0;
	gun = new SimpleGun(0, "Pistol", 0, 0, iBulletPoolId, 0.5, 0.1, 5 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	//		add more gun. Max is 9 type of guns
	gun = new AutoGun(1, "AK", 0, 0, iBulletPoolId, 0.5, 0.75, 0.05, 5*M_PI/180, 10*M_PI/180, 1, 5);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(2, "Shotgun", 0, 0, iBulletPoolId, 0.5, 0.1, 60 * M_PI / 180, 5);
	m_combatController->AddWeapon(gun);
	gun = new AutoGun(3, "Uzi", 0, 0, iBulletPoolId, 0.5, 0.75, 0.05, 15 * M_PI / 180, 30 * M_PI / 180, 1, 7);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(4, "Sniper", 0, 0, iBulletPoolId, 0.5, 0.1, 0 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	iBulletPoolId = 1;
	gun = new SimpleGun(5, "Cannon", 0, 0, iBulletPoolId, 0.5, 0.1, 5 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(6, "SuperCannon", 0, 0, iBulletPoolId, 0.5, 0.1, 45 * M_PI / 180, 3);
	m_combatController->AddWeapon(gun);
	// give player some bullets when start game
	m_combatController->AddBullet(0, 500);
	m_combatController->AddBullet(1, 200);
	

	// test UI. For all UIcomponent, set camera-position-z < UI-position-z < 0
	UIComponent* ui;
	
	position = Vector3(0, 0, -1);
	rotation = 0;
	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1.0;
	iMaterialId = 0;
	iMainTexId = 15; // black bullet

	ui = new UIComponent(-1);
	ui->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	ui->SetBound(1, -1, -1, 1);
	ui->SetRenderType(UIComponent::RenderType::Expand);
	AddUIComponent(ui);

	iMainTexId = 10; // black bullet
	ui = new UIComponent(-1);
	ui->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	ui->SetBound(0.5, -0.5, -0.5, 0.5);
	ui->SetRenderType(UIComponent::RenderType::Expand);
	AddUIComponent(ui);
	// TODO: delete this when finish testing

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
		FloatingFish* fish = new FloatingFish(iObjectId,numOfTarget,Target);
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
	Sprite* backGround = new Sprite(iObjectId);
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
	Sprite* name = new Sprite(iObjectId);
	name->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(name, MENU_OBJECT);

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
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		Button* button = new Button(iObjectId);
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

		// TODO: testing UIComponent. Delete this when finish testing
		//m_ui->SetTop(m_ui->GetTop() + 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N1) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N2)));
		//m_ui->SetBottom(m_ui->GetBottom() + 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N3) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N4)));
		//m_ui->SetLeft(m_ui->GetLeft() + 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N5) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N6)));
		//m_ui->SetRight(m_ui->GetRight() + 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N7) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N8)));
		//m_ui->SetRotation(m_ui->GetRotation() + 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N9) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N0)));

		Vector3 newPos = m_listUIComponents[0]->GetPosition();
		newPos.x += 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::RIGHT) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LEFT));
		newPos.y += 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::UP) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::DOWN));
		newPos.z += 0.01 * (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT));
		m_listUIComponents[0]->SetPosition(newPos);
		printf("%f\n", newPos.z);

		//if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::TAB)) {
		//	m_ui->SetRenderType(UIComponent::RenderType::Fit);
		//}
		//else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT)) {
		//	m_ui->SetRenderType(UIComponent::RenderType::Expand);
		//}
		//else if(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE)) {
		//	m_ui->SetRenderType(UIComponent::RenderType::Stretch);
		//}
		


		m_time += frameTime;
		Singleton<WorldManager>::GetInstance()->Update(frameTime);
		for (int i = 0; i < m_listObject.size(); i++) {
			if (m_listObject[i]->CheckIsActiveSprite())
				m_listObject[i]->Update(frameTime);
		}
		for (int i = 0; i < m_listUIComponents.size(); i++) {
			if (m_listUIComponents[i]->CheckIsActiveSprite())
				m_listUIComponents[i]->Update(frameTime);
		}
		m_mainCamera->Update(frameTime);
		m_combatController->Update(frameTime);
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
			if (m_listObject[i]->CheckIsActiveSprite())
				m_listObject[i]->Render(m_mainCamera);
		}
		for (int i = 0; i < m_listUIComponents.size(); i++) {
			if (m_listUIComponents[i]->CheckIsActiveSprite())
				m_listUIComponents[i]->Render(m_mainCamera);
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
void SceneManager2D::AddUIComponent(UIComponent* object) {
	if (m_listUIComponents.size() == 0) {
		m_listUIComponents.push_back(object);
		return;
	}
	float zPos = object->GetPosition().z;
	if (zPos >= m_listUIComponents[0]->GetPosition().z) {
		m_listUIComponents.insert(m_listUIComponents.begin(), object);
		return;
	}
	for (int i = 1;i < m_listUIComponents.size();i++) {
		if (m_listUIComponents[i - 1]->GetPosition().z >= zPos && zPos >= m_listUIComponents[i]->GetPosition().z) {
			m_listUIComponents.insert(m_listUIComponents.begin() + i, object);
			return;
		}
	}
	m_listUIComponents.push_back(object);
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
