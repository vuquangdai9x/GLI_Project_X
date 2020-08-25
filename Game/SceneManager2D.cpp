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
#include"SuicideBug.h"
#include "RockyGolem.h"
#include "DeadlyBird.h"
#include "VampireBat.h"

#include "SimpleGun.h"
#include "AutoGun.h"
#include "GunBulletPool.h"

#include "UIComponent.h"
#include "UIText.h"

SceneManager2D::~SceneManager2D()
{
	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->OnDestroy();
		delete m_listObject[i];
	}
	if (m_mainCamera != NULL) delete m_mainCamera;
	if (m_menuCamera!= NULL) delete m_menuCamera;
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
	m_curent = player;
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
	iMainTexId = 55101; // red bullet
	iPoolCapacity = 500;
	GunBullet* templateGunBullet = new GunBullet(-1, 1.0, 0.0, 5.0, 30.0, 2);
	templateGunBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(0, iPoolCapacity, 10, templateGunBullet));

	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1.0;
	iMaterialId = 0;
	iMainTexId = 55201; // black bullet
	iPoolCapacity = 200;
	GunBullet* templateCannonBullet = new GunBullet(-1, 3.0, 1.0, 7.0, 15.0, 3);
	templateCannonBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	m_combatController->AddBulletPool(new GunBulletPool(1, iPoolCapacity, 10, templateCannonBullet));

	// add guns
	//		standard example
	Weapon* gun;
	int iBulletPoolId;
	iBulletPoolId = 0;
	gun = new SimpleGun(0, "Pistol", 50301, 0, iBulletPoolId, 0.5, 0.1, 5 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	//		add more gun. Max is 9 type of guns
	gun = new AutoGun(1, "AK", 50101, 60201, iBulletPoolId, 0.5, 0.75, 0.05, 5*M_PI/180, 10*M_PI/180, 1, 5);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(2, "Shotgun", 50501, 60201, iBulletPoolId, 0.5, 0.1, 60 * M_PI / 180, 5);
	m_combatController->AddWeapon(gun);
	gun = new AutoGun(3, "Uzi", 50701, 60201, iBulletPoolId, 0.5, 0.75, 0.05, 15 * M_PI / 180, 30 * M_PI / 180, 1, 7);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(4, "Sniper", 50601, 60201, iBulletPoolId, 0.5, 0.1, 0 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	iBulletPoolId = 1;
	gun = new SimpleGun(5, "Cannon", 50201, 60201, iBulletPoolId, 0.5, 0.1, 5 * M_PI / 180, 1);
	m_combatController->AddWeapon(gun);
	gun = new SimpleGun(6, "SuperCannon", 50201, 60201, iBulletPoolId, 0.5, 0.1, 45 * M_PI / 180, 3);
	m_combatController->AddWeapon(gun);
	// give player some bullets when start game
	m_combatController->AddBullet(0, 500);
	m_combatController->AddBullet(1, 200);
	

	position = Vector3(0, 0, -1);
	rotation = 0;
	scale = Vector2(1.0, 1.0);
	uiHexColor = 0xffffff;
	alpha = 1;
	iMaterialId = 0;
	iMainTexId = 15;

	float healthBarScale = 100;
	UIComponent* healthIcon = new UIComponent(-1);
	iMainTexId = 60101;
	uiHexColor = 0xE84545;
	healthIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthIcon);
	UIComponent* healthBar = new UIComponent(-1);
	iMainTexId = 60102;
	uiHexColor = 0xE84545;
	healthBar->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthBar);
	UIComponent* healthDecorate = new UIComponent(-1);
	iMainTexId = 60101;
	uiHexColor = 0xE84545;
	healthDecorate->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthDecorate);
	
	UIComponent* weaponIcon = new UIComponent(-1);
	iMainTexId = 50401;
	uiHexColor = 0xFFFFFF;
	weaponIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(weaponIcon);

	AnimationController& animCtrl = weaponIcon->GetAnimationController();
	int listTextAnim[] = { 50101,50201,50301,50401,50501,50601,50701 };
	animCtrl.AddAnimState(0, 15, 4, 6, 6);
	animCtrl.AddAnimState(1, 7, listTextAnim, 7);
	animCtrl.SetDefaultAnimState(1);
	animCtrl.RunAnimState(0, 2);
	//animCtrl.RunAnimState(1, 3);
	weaponIcon->SetUseAnimation(true);

	UIComponent* targetIcon = new UIComponent(-1);
	iMainTexId = 60203;
	uiHexColor = 0xE84545;
	targetIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(targetIcon);
	
	iMaterialId = 1;
	UIText* bulletStatus = new UIText(-1);
	iMainTexId = -1;
	uiHexColor = 0xFFFFFF;
	bulletStatus->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	bulletStatus->SetText("45/250");
	bulletStatus->SetFont(2);
	AddObject(bulletStatus);
	UIText* score = new UIText(-1);
	iMainTexId = -1;
	uiHexColor = 0xFFFFFF;
	score->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	score->SetFont(2);
	score->SetText("Score: 6996");
	AddObject(score);

	m_HUDController = new HUDController();
	m_HUDController->Init(healthBarScale, healthIcon, healthBar, healthDecorate, bulletStatus, weaponIcon, targetIcon, score);
	
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

		printf("[msg] SceneManager: Loaded Sprite %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
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
	fscanf(fIn, "SUICIDEBUG %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "NUM OF TARGET %d\n", &numOfTarget);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		SuicideBug* bug = new SuicideBug(iObjectId);
		bug->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		bug->createBox2D();
		//bug->SetIgnore();
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		AddObject(bug);
	}

	fscanf(fIn, "ROCKYGOLEM %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		RockyGolem* rocky = new RockyGolem(iObjectId);
		rocky->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		rocky->createBox2D();
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		AddObject(rocky);
	}
	fscanf(fIn, "DEADLYBIRD %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		DeadlyBird* bird = new DeadlyBird(iObjectId);
		bird->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		bird->createBox2D();
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		AddObject(bird);
	}
	fscanf(fIn, "VAMPIREBAT %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		Vector3 startPos, endPos;
		fscanf(fIn, "START POSITION %f %f %f\n", &(startPos.x), &(startPos.y), &(startPos.z));
		fscanf(fIn, "END POSITION %f %f %f\n", &(endPos.x), &(endPos.y), &(endPos.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		VampireBat* bat = new VampireBat(iObjectId, Vector2(startPos.x, startPos.y), Vector2(endPos.x, endPos.y));
		bat->Init(startPos, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		bat->createBox2D();
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		AddObject(bat);
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
		m_time += frameTime;
		for (int i = 0; i < m_listObject.size(); i++) {
			if (m_listObject[i]->CheckIsActiveSprite())
				m_listObject[i]->Update(frameTime);
		}
		m_mainCamera->Update(frameTime);
		m_combatController->Update(frameTime);
		Singleton<WorldManager>::GetInstance()->Update(frameTime);
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

void SceneManager2D::RemoveObject(Sprite* object)
{
	for (int i = 0;i < m_listObject.size();i++) {
		if (m_listObject[i] == object) {
			m_listObject.erase(m_listObject.begin()+i);
			break;
		}
	}
}

std::vector<Sprite*>& SceneManager2D::GetListObject() {
	return m_listObject;
}

void SceneManager2D::getPlayerPos(Vector3 &pos)
{
	pos= m_curent->GetPosition();
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
