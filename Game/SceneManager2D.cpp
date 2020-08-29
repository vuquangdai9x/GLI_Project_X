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

#include "EffectManager.h"

SceneManager2D::SceneManager2D()
{
	m_mainCamera = NULL;
	m_menuCamera = NULL;
	m_currentPlayer = NULL;
	m_combatController = NULL;
	m_time = 0;
}

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

bool SceneManager2D::LoadAnimation(FILE* fIn, Sprite* sprite) {
	int iNumOfAnimations;
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	if (iNumOfAnimations > 0) {
		AnimationController& animCtrl = sprite->GetAnimationController();
		int iDefaultAnimId;
		fscanf(fIn, "DEFAULT %d\n", &iDefaultAnimId);
		int iCountValidAnim = 0;
		for (int i = 0;i < iNumOfAnimations;i++) {
			int iAnimStateId;
			char animType[20];
			float cycleTime;
			int iAnimTexId;
			int iSplitX, iSplitY;
			int iNumOfAnimTextures;
			int* listTextAnim;
			fscanf(fIn, "ANIM %d\n", &iAnimStateId);
			fscanf(fIn, "TYPE %s\n", animType);
			fscanf(fIn, "CYCLE TIME %f\n", &cycleTime);

			if (strcmp("SPLIT", animType) == 0) {
				fscanf(fIn, "TEXTURE %d\n", &iAnimTexId);
				fscanf(fIn, "SPLITXY %d %d\n", &iSplitX, &iSplitY);
				animCtrl.AddAnimState(iAnimStateId, iAnimTexId, iSplitX, iSplitY, cycleTime);
				iCountValidAnim++;
			}
			else if (strcmp("MULTITEX", animType) == 0) {
				//TEXTURES 7: 50101 50201 50301 50401 50501 50601 50701
				fscanf(fIn, "TEXTURES %d:", &iNumOfAnimTextures);
				listTextAnim = new int[iNumOfAnimTextures];
				for (int j = 0;j < iNumOfAnimTextures;j++) {
					fscanf(fIn, " %d", &(listTextAnim[j]));
				}
				fscanf(fIn, "\n");
				animCtrl.AddAnimState(iAnimStateId, iNumOfAnimTextures, listTextAnim, cycleTime);
				delete[] listTextAnim;
				iCountValidAnim++;
			}
			else {
				printf("[ERR] SceneManager2D:LoadAnimation: AnimId %d: type invalid: %s", iAnimStateId, animType);
			}
		}

		if (iCountValidAnim > 0) {
			printf("[msg] SceneManager2D:LoadAnimation: Loaded %d animation\n", iCountValidAnim);
			animCtrl.SetDefaultAnimState(iDefaultAnimId);
			sprite->SetUseAnimation(true);
			return true;
		}
		else {
			printf("[ERR] SceneManager2D:LoadAnimation: No animation valid\n");
			return false;
		}
	}
	return false;
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
	int iNumOfObject, iObjectId;
	int iMaterialId;
	int iMainTexId;
	int iFontId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	char shapeType[10];

	// Set up player
	fscanf(fIn, "#PLAYER %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);

	Player* player = new Player(iObjectId);
	player->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	player->createBox2D();
	AddObject(player);
	m_currentPlayer = player;
	
	LoadAnimation(fIn, player);
	
	printf("[msg] SceneManager2D: Init player\n");

	// set up HUD
	fscanf(fIn, "\n#HUD\n", &iObjectId);
	fscanf(fIn, "+ HEALTH ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);

	UIComponent* healthIcon = new UIComponent(-1);
	healthIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthIcon);
	LoadAnimation(fIn, healthIcon);
	
	printf("[msg] SceneManager2D: HUD | Init health icon\n");

	fscanf(fIn, "+ HEALTH BAR\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	UIComponent * healthBar = new UIComponent(-1);
	healthBar->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthBar);
	LoadAnimation(fIn, healthBar);

	printf("[msg] SceneManager2D: HUD | Init health bar\n");

	fscanf(fIn, "+ HEALTH DECORATE\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	UIComponent * healthDecorate = new UIComponent(-1);
	healthDecorate->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(healthDecorate);
	LoadAnimation(fIn, healthDecorate);

	printf("[msg] SceneManager2D: HUD | Init health decorate\n");

	fscanf(fIn, "+ WEAPON ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	UIComponent * weaponIcon = new UIComponent(-1);
	weaponIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(weaponIcon);

	printf("[msg] SceneManager2D: HUD | Init weapon icon\n");

	fscanf(fIn, "+ TARGET ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	UIComponent * targetIcon = new UIComponent(-1);
	targetIcon->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	AddObject(targetIcon);

	printf("[msg] SceneManager2D: HUD | Init target icon\n");

	fscanf(fIn, "+ BULLET STATUS\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	UIText * bulletStatus = new UIText(-1);
	bulletStatus->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	bulletStatus->SetFont(iFontId);
	AddObject(bulletStatus);
	printf("[msg] SceneManager2D: HUD | Init bullet status textbox\n");

	fscanf(fIn, "+ SCORE\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);
	UIText * score = new UIText(-1);
	score->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	score->SetFont(iFontId);
	AddObject(score);
	printf("[msg] SceneManager2D: HUD | Init score textbox\n");
	score->SetText("Score: 6996");


	HUDController * m_HUDController = new HUDController();
	m_HUDController->Init(100, healthIcon, healthBar, healthDecorate, bulletStatus, weaponIcon, targetIcon, score);
	player->setHUDController(m_HUDController);
	printf("[msg] SceneManager2D: HUD | Done setup HUDController\n");

	// Set up weapon & bullet
	int iNumOfPlayerBullet;
	m_combatController = new CombatController(player);

	fscanf(fIn, "#PLAYER_BULLET %d\n", &iNumOfPlayerBullet);
	for (int i = 0;i < iNumOfPlayerBullet;i++) {
		int iPoolId;
		int iPoolCapacity, iPoolInitCount;
		float bulletMass, bulletDamage, bulletInitSpeed, bulletGravityScale, bulletExistTime;
		fscanf(fIn, "ID %d\n", &iPoolId);
		fscanf(fIn, "INIT %d\n", &iPoolInitCount);
		fscanf(fIn, "CAPACITY %d\n", &iPoolCapacity);
		fscanf(fIn, "MASS %f\n", &bulletMass);
		fscanf(fIn, "GRAVITY SCALE %f\n", &bulletGravityScale);
		fscanf(fIn, "DAMAGE %f\n", &bulletDamage);
		fscanf(fIn, "INIT SPEED %f\n", &bulletInitSpeed);
		fscanf(fIn, "EXIST TIME %f\n", &bulletExistTime);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		GunBullet* templateGunBullet = new GunBullet(-1, bulletMass, bulletGravityScale, bulletDamage, bulletInitSpeed, bulletExistTime);
		templateGunBullet->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		LoadAnimation(fIn, templateGunBullet);

		m_combatController->AddBulletPool(new GunBulletPool(iPoolId, iPoolCapacity, 10, templateGunBullet));
		m_combatController->AddBullet(iPoolId, iPoolInitCount);

		printf("[msg] SceneManager2D: Init Bullet Pool %d\n", iPoolId);
	}

	// add guns
	int iNumOfWeapons;
	fscanf(fIn, "#WEAPON %d\n", &iNumOfWeapons);
	for (int i = 0;i < iNumOfWeapons;i++) {
		Weapon* gun;

		int iWeaponId;
		char weaponType[20];
		char weaponName[255];
		int iWeaponTexId, iTargetTexId;
		int iBulletType;
		float oppositeForce;
		float rechargeTime, shortRechargeTime;
		float randomAngle, spreadRandomAngle;
		int iFireAtOnce, iFireAmount;
		fscanf(fIn, "ID %d\n", &iWeaponId);
		fscanf(fIn, "TYPE %s\n", weaponType);
		fscanf(fIn, "NAME %s\n", weaponName);
		fscanf(fIn, "WEAPON TEX %d\n", &iWeaponTexId);
		fscanf(fIn, "TARGET TEX %d\n", &iTargetTexId);
		fscanf(fIn, "BULLET TYPE %d\n", &iBulletType);
		fscanf(fIn, "OPPOSITE FORCE %f\n", &oppositeForce);
		fscanf(fIn, "RECHARGE TIME %f\n", &rechargeTime);
		fscanf(fIn, "RANDOM ANGLE %f\n", &randomAngle);
		randomAngle = randomAngle * M_PI / 180;
		fscanf(fIn, "FIRE AT ONCE %d\n", &iFireAtOnce);

		if (strcmp("SIMPLEGUN", weaponType) == 0) {
			gun = new SimpleGun(iWeaponId, weaponName, iWeaponTexId, iTargetTexId, iBulletType, oppositeForce, rechargeTime, randomAngle, iFireAtOnce);
			m_combatController->AddWeapon(gun);

			printf("[msg] SceneManager2D: Init simple gun %d: %s\n", iWeaponId, weaponName);
		}else if (strcmp("AUTOGUN", weaponType) == 0) {
			fscanf(fIn, "SHORT RECHARGE TIME %f\n", &shortRechargeTime);
			fscanf(fIn, "SPREAD ANGLE %f\n", &spreadRandomAngle);
			spreadRandomAngle = spreadRandomAngle * M_PI / 180;
			fscanf(fIn, "FIRE AMOUNT %d\n", &iFireAmount);
			gun = new AutoGun(iWeaponId, weaponName, iWeaponTexId, iTargetTexId, iBulletType, oppositeForce, rechargeTime, shortRechargeTime, randomAngle, spreadRandomAngle, iFireAtOnce, iFireAmount);
			m_combatController->AddWeapon(gun);

			printf("[msg] SceneManager2D: Init auto gun %d: %s\n", iWeaponId, weaponName);
		}
		else {
			printf("[ERR] SceneManager2D: Weapon %d: type invalid: %s", iWeaponId, weaponType);
		}
	}
	m_combatController->ChangeWeapon(0);
	
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
		
		LoadAnimation(fIn, obs);
		if (strcmp(shapeType, "RECT") == 0) {
			obs->createBox2D();
		}
		else {
			obs->createTriangle2D();
		}
		AddObject(obs);
		printf("[msg] SceneManager: Loaded Obstacle %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}
	int numOfTarget;
	b2Vec2 Target[10];

	fscanf(fIn, "\n#ENEMY\n");
	fscanf(fIn, "#FLOATINGFISH\n");

	// use template fish for reduce animation writing for each object in scene.txt
	FloatingFish templateFish(-1,1,Target);
	LoadAnimation(fIn, &templateFish);

	fscanf(fIn, "COUNT %d\n", &iNumOfObject);
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
		
		// clone animation from template
		fish->GetAnimationController().Clone(templateFish.GetAnimationController());
		fish->SetUseAnimation(templateFish.CheckUseAnimation());

		AddObject(fish);

		printf("[msg] SceneManager: Loaded FloatingFish %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}

	fscanf(fIn, "#SUICIDEBUG\n");
	// use template fish for reduce animation writing for each object in scene.txt
	SuicideBug templateBug(-1);
	LoadAnimation(fIn, &templateBug);

	fscanf(fIn, "COUNT %d\n", &iNumOfObject);
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
		AddObject(bug);

		bug->GetAnimationController().Clone(templateBug.GetAnimationController());
		bug->SetUseAnimation(templateBug.CheckUseAnimation());

		printf("[msg] SceneManager: Loaded SuicideBug %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}

	fscanf(fIn, "#ROCKYGOLEM\n");
	// use template fish for reduce animation writing for each object in scene.txt
	RockyGolem templateGolem(-1);
	LoadAnimation(fIn, &templateGolem);

	fscanf(fIn, "COUNT %d\n", &iNumOfObject);
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
		AddObject(rocky);

		rocky->GetAnimationController().Clone(templateGolem.GetAnimationController());
		rocky->SetUseAnimation(templateGolem.CheckUseAnimation());

		printf("[msg] SceneManager: Loaded RockyGolem %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}

	fscanf(fIn, "#DEADLYBIRD\n");
	// use template fish for reduce animation writing for each object in scene.txt
	DeadlyBird templateBird(-1);
	LoadAnimation(fIn, &templateBird);

	fscanf(fIn, "COUNT %d\n", &iNumOfObject);
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
		AddObject(bird);

		bird->GetAnimationController().Clone(templateBird.GetAnimationController());
		bird->SetUseAnimation(templateBird.CheckUseAnimation());

		printf("[msg] SceneManager: Loaded DeadlyBird %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
	}

	fscanf(fIn, "#VAMPIREBAT\n");
	// use template fish for reduce animation writing for each object in scene.txt
	VampireBat templateBat(-1, Vector2(0,0), 1);
	LoadAnimation(fIn, &templateBat);

	fscanf(fIn, "COUNT %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		Vector3 startPos;
		float range;
		fscanf(fIn, "CENTER POSITION %f %f %f\n", &(startPos.x), &(startPos.y), &(startPos.z));
		fscanf(fIn, "RANGE %f\n", &range);
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		VampireBat* bat = new VampireBat(iObjectId, Vector2(startPos.x, startPos.y), range);
		bat->Init(startPos, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		bat->createBox2D();
		AddObject(bat);

		bat->GetAnimationController().Clone(templateBat.GetAnimationController());
		bat->SetUseAnimation(templateBat.CheckUseAnimation());

		printf("[msg] SceneManager: Loaded VampireBat %d | Material: %d | Main Texture: %d\n", iObjectId, iMaterialId, iMainTexId);
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
			if (fabs(m_listObject[i]->GetPosition().y - m_mainCamera->GetPosition().y) > 100) continue;
			if (m_listObject[i]->CheckIsActiveSprite())
				m_listObject[i]->Update(frameTime);
		}
		m_mainCamera->Update(frameTime);
		m_combatController->Update(frameTime);
		Singleton<EffectManager>::GetInstance()->Update(frameTime);
		Singleton<WorldManager>::GetInstance()->Update(frameTime);

		printf("y = %f\n", m_mainCamera->GetPosition().y);
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
			if (fabs(m_listObject[i]->GetPosition().y - m_mainCamera->GetPosition().y) > 100) continue;
			if (m_listObject[i]->CheckIsActiveSprite())
				m_listObject[i]->Render(m_mainCamera);
		}
		Singleton<EffectManager>::GetInstance()->Render(m_mainCamera);
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
	pos= m_currentPlayer->GetPosition();
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
