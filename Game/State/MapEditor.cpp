#include"MapEditor.h"
#include "GS_PauseState.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include"../Singleton.h"
#include"GameStateManager.h"
#include "../ResourcesManager2D.h"
#include "../SceneManager2D.h"
#include "../Obstacle.h"
#include "../SuicideBug.h"
#include "../FloatingFish.h"
#include "../DeadlyBird.h"
#include"../RockyGolem.h"
#include"../VampireBat.h"
void getInfor() {

}
MapEditor::MapEditor()
{
	
	char sceneFile[50] = "Datas/scene2d-map.txt";
	char resourcesFile[60] = "Datas/resources2d.txt";
	m_play = new Player(0);
	m_play->SetPosition(Vector3(0, 0, 8));
	//Singleton<ResourceManager2D>::GetInstance()->LoadResources(resourcesFile);
	m_curent = NULL;
	m_currentSprite = NULL;
	button = Singleton<SceneManager2D>::GetInstance()->LoadMapScene(sceneFile);
	{m_victoryPoint = new Sprite(0);
	int iNumOfObject, iObjectId;
	int iMaterialId;
	int iMainTexId;
	int iFontId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	iMaterialId = 0;
	iMainTexId = 60401;
	rotation = 0;
	scale.x = 1;
	scale.y = 1;
	uiHexColor = 0xffff00;
	alpha = 1;
	position.x = 0;
	position.y = 17;
	position.z = 9;
	m_victoryPoint->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	Singleton<SceneManager2D>::GetInstance()->AddObject(m_victoryPoint, MAP_OBJECT); }
	loadMap("Datas/map-load.txt");
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->UpdateMember();
		button[i]->setBuffer(&m_curent);
		if (button[i]->m_infor.type == OBSTACLE_UNIT) {
			pushBackBoxType(button[i]->m_infor.mainTex, button[i]->m_infor.boxType);
		}
	}
	
}

MapEditor::~MapEditor()
{
	delete m_play;
	delete bgr, borderLeft, borderRight;
	delete m_victoryPoint;
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_victoryPoint);
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(bgr);
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(borderLeft);
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(borderRight);
	Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_play);
	for (int i = 0;i < m_Obstacle.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_Obstacle[i]);
		delete m_Obstacle[i];
	}
	for (int i = 0;i < m_floatingFish.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_floatingFish[i]);
		delete m_floatingFish[i];
	}
	for (int i = 0;i < m_suicudeBug.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_suicudeBug[i]);
		delete m_suicudeBug[i];
	}
	for (int i = 0;i < m_deadlyBird.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_deadlyBird[i]);
		delete m_deadlyBird[i];
	}
	for (int i = 0;i < m_rockyGolem.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_rockyGolem[i]);
		delete m_rockyGolem[i];
	}
	for (int i = 0;i < m_vampireBat.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_vampireBat[i]);
		delete m_vampireBat[i];
	}
	for (int i = 0;i < m_fLoatingFishMove.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_fLoatingFishMove[i]);
		delete m_fLoatingFishMove[i];
	}
	for (int i = 0;i < button.size();i++) {
		Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(button[i]);
		delete button[i];
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

void MapEditor::loadMap(char* dataSceneFile)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataSceneFile);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("Fails to load scene file");
		return;
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

	// set up map 
	char mapName[256];
	float mapWidth;
	int height;
	fscanf(fIn, "#MAP %s\n", mapName);
	fscanf(fIn, "WIDTH %f\n", &mapWidth);
	fscanf(fIn, "HEIGHTWIN %d\n", &height);
	Vector3 vicPos = m_victoryPoint->GetPosition();
	vicPos.y = (float)height;
	m_victoryPoint->SetPosition(vicPos);
	// Set up player
	fscanf(fIn, "#PLAYER %d\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "ROTATION %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "MISSON %*d\n");
	int iLoopVertical, iLoopHorizontal;
	int iNumOfBackground;
	float scaleFactor;
	fscanf(fIn, "#BACKGROUNDS %d\n", &iNumOfBackground);
	for (int i = 0;i < iNumOfBackground;i++) {
		fscanf(fIn, "ID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		fscanf(fIn, "LOOP %d %d\n", &iLoopHorizontal, &iLoopVertical);
		bgr = new MapBorder(iObjectId, this->m_play, 0);
		position.x = m_play->GetPosition().x;
		bgr->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		bgr->SetLoopAmount(iLoopHorizontal, iLoopVertical);
		Singleton<SceneManager2D>::GetInstance()->AddObject(bgr, MAP_OBJECT);
		int iNumOfAnimations;
		fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
		scaleFactor = mapWidth / bgr->GetOriginSize().x / 2;
		bgr->SetScale(bgr->GetScale() * scaleFactor);

		printf("[msg] SceneManager2D: Load background %d\n", iObjectId);
	}

	// Set up map border
	// border left
	float borderDamage;
	fscanf(fIn, "#BORDER LEFT\n");
	fscanf(fIn, "ID %d\n", &iObjectId);
	fscanf(fIn, "DAMAGE %f\n", &borderDamage);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "LOOP %d %d\n", &iLoopHorizontal, &iLoopVertical);
	fscanf(fIn, "ANIMATIONS 0\n");
	borderLeft = new MapBorder(iObjectId, m_play, borderDamage);
	position.x = m_play->GetPosition().x - mapWidth / 2;
	borderLeft->Init(position, 0, scale * scaleFactor, uiHexColor, alpha, iMaterialId, iMainTexId);
	borderLeft->createCollider();
	Singleton<SceneManager2D>::GetInstance()->AddObject(borderLeft, MAP_OBJECT);
	borderLeft->SetLoopAmount(iLoopHorizontal, iLoopVertical);



	fscanf(fIn, "#BORDER RIGHT\n");
	fscanf(fIn, "ID %d\n", &iObjectId);
	fscanf(fIn, "DAMAGE %f\n", &borderDamage);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "LOOP %d %d\n", &iLoopHorizontal, &iLoopVertical);
	fscanf(fIn, "ANIMATIONS 0\n");
	borderRight = new MapBorder(iObjectId, m_play, borderDamage);
	position.x = m_play->GetPosition().x + mapWidth / 2;
	borderRight->Init(position, 0, scale * scaleFactor, uiHexColor, alpha, iMaterialId, iMainTexId);
	borderRight->createCollider();
	Singleton<SceneManager2D>::GetInstance()->AddObject(borderRight, MAP_OBJECT);
	borderRight->SetLoopAmount(iLoopHorizontal, iLoopVertical);

	// set up HUD
	fscanf(fIn, "\n#HUD\n", &iObjectId);
	fscanf(fIn, "+ HEALTH ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS 0\n");


	fscanf(fIn, "+ HEALTH BAR\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS 0\n");

	printf("[msg] SceneManager2D: HUD | Init health bar\n");

	fscanf(fIn, "+ HEALTH DECORATE\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "ANIMATIONS 0\n");


	fscanf(fIn, "+ WEAPON ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);



	fscanf(fIn, "+ TARGET ICON\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);


	fscanf(fIn, "+ BULLET STATUS\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);

	fscanf(fIn, "+ SCORE\n", &iObjectId);
	fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
	fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
	fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
	fscanf(fIn, "FONT %d\n", &iFontId);


	int iNumOfPlayerBullet;
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
		fscanf(fIn, "ANIMATIONS 0\n");
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
		if (strcmp("AUTOGUN", weaponType) == 0) {
			fscanf(fIn, "SHORT RECHARGE TIME %f\n", &shortRechargeTime);
			fscanf(fIn, "SPREAD ANGLE %f\n", &spreadRandomAngle);
			fscanf(fIn, "FIRE AMOUNT %d\n", &iFireAmount);
		}
	}


	//
	// set up other object
	fscanf(fIn, "OBSTACLE_TYPE_0 %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		fscanf(fIn, "\nID %d\n", &iObjectId);
		Sprite* obs = new Obstacle(iObjectId, 0);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		obs->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		fscanf(fIn, "TYPE %s\n", shapeType);
		fscanf(fIn, "ANIMATIONS 0\n");
		Singleton<SceneManager2D>::GetInstance()->AddObject(obs, MAP_OBJECT);
		this->m_Obstacle.push_back(obs);
	}
	int numOfTarget;
	b2Vec2 Target[10];

	fscanf(fIn, "\n#ENEMY\n");
	fscanf(fIn, "#FLOATINGFISH\n");
	loadAnimation(fIn);
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
		Sprite* fish = new FloatingFish(iObjectId);
		fish->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		for (int j = 0; j < numOfTarget; j++) {
			Sprite* tmp = createX();
			dynamic_cast<FloatingFishMoveSet*>(tmp)->fish = fish;
			dynamic_cast<FloatingFishMoveSet*>(tmp)->distanceToFish = Vector2(Target[j].x - position.x, Target[j].y - position.y);
			m_fLoatingFishMove.push_back(tmp);
			Singleton<SceneManager2D>::GetInstance()->AddObject(tmp, MAP_OBJECT);
		}
		m_floatingFishMoveNum.push_back(numOfTarget);
		Singleton<SceneManager2D>::GetInstance()->AddObject(fish, MAP_OBJECT);
		m_floatingFish.push_back(fish);

	}

	fscanf(fIn, "#SUICIDEBUG\n");
	loadAnimation(fIn);

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
		Sprite* bug = new SuicideBug(iObjectId);
		bug->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		//bug->SetIgnore();
		Singleton<SceneManager2D>::GetInstance()->AddObject(bug, MAP_OBJECT);
		m_suicudeBug.push_back(bug);


	}

	fscanf(fIn, "#ROCKYGOLEM\n");
	loadAnimation(fIn);

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
		Sprite* rocky = new RockyGolem(iObjectId);
		rocky->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		Singleton<SceneManager2D>::GetInstance()->AddObject(rocky, MAP_OBJECT);
		m_rockyGolem.push_back(rocky);

	}

	fscanf(fIn, "#DEADLYBIRD\n");
	// use template fish for reduce animation writing for each object in scene.txt
	loadAnimation(fIn);

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
		Sprite* bird = new DeadlyBird(iObjectId);
		bird->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		Singleton<SceneManager2D>::GetInstance()->AddObject(bird, MAP_OBJECT);
		m_deadlyBird.push_back(bird);


	}

	fscanf(fIn, "#VAMPIREBAT\n");
	// use template fish for reduce animation writing for each object in scene.txt
	loadAnimation(fIn);

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
		Sprite* bat = new VampireBat(iObjectId, Vector2(startPos.x, startPos.y), range);
		bat->Init(startPos, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		Singleton<SceneManager2D>::GetInstance()->AddObject(bat, MAP_OBJECT);
		m_vampireBat.push_back(bat);
	}


	float nearPlane, farPlane, zoom;

	fscanf(fIn, "\n#CAMERA\n");
	fscanf(fIn, "NEAR %f\n", &nearPlane);
	fscanf(fIn, "FAR %f\n", &farPlane);
	fscanf(fIn, "ZOOM %f\n", &zoom);
	fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
	fscanf(fIn, "DUTCH %f\n", &rotation);
	rotation = rotation * 2 * M_PI / 360;
	fclose(fIn);



}


void MapEditor::Render()
{
	Singleton<SceneManager2D>::GetInstance()->Render(MAP_OBJECT);
}

void MapEditor::Update(float deltaTime)
{

	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::ESCAPE)) {
		saveFile("Datas/map-out.txt");
		Singleton<GameStateManager>::GetInstance()->DeleteMap();
		return;
	}
	if (m_curent != NULL) {
		addObject();
		m_curent = NULL;



	}
	for (int i = 0; i < this->button.size(); i++) {
		button[i]->Update(deltaTime);
	}
	for (int i = 0;i < this->m_fLoatingFishMove.size();i++) {
		m_fLoatingFishMove[i]->Update(deltaTime);
	}
	{

	}
	Camera2D& camera = Singleton<SceneManager2D>::GetInstance()->GetMainCamera(MAP_OBJECT);
	Vector3 cameraPos = camera.GetPosition();
	cameraPos.y += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::W) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::S)) * deltaTime * camera.GetZoom();
	cameraPos.x += ((Singleton<InputManager>::GetInstance()->GetBit(InputManager::D) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::A))) * deltaTime * camera.GetZoom();
	camera.Dutch(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::E) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Q), deltaTime);
	camera.Zoom(Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::X) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::Z), deltaTime * 10);
	camera.SetPosition(cameraPos);
	m_play->SetPosition(cameraPos);

	{
		bgr->Update(deltaTime);borderLeft->Update(deltaTime);borderRight->Update(deltaTime);
	}
	if (m_floatingCreated) {
		if (m_currentSprite == NULL) {
			m_currentSprite = createX(); Singleton<SceneManager2D>::GetInstance()->AddObject(m_currentSprite, MAP_OBJECT);
		}
		else if (m_currentSprite != NULL && (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_MOVE || Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_RELEASE)) {
			int iMousePosX, iMousePosY, pre_x, pre_y;
			Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
			Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
			Vector3 Pos = m_currentSprite->GetPosition();
			Pos.x = pos3D.x;
			Pos.y = pos3D.y;
			m_currentSprite->SetPosition(Pos);
			int checkMaxMove = 1;
			int currentFish = m_floatingFish.size() - 1;
			if (m_floatingFishMoveNum.size() == currentFish);else {
				if (m_floatingFishMoveNum[currentFish] == 10) checkMaxMove = 0;
			}

			if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE) || !checkMaxMove) {
				if(m_currentSprite==m_victoryPoint){}
				else {
					m_floatingCreated = false;
					Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_currentSprite);
					delete m_currentSprite;
					m_currentSprite = NULL;
				}
			}
		}
		else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK && m_currentSprite != NULL) {

			int currentFish = m_floatingFish.size() - 1;
			if (m_floatingFishMoveNum.size() == currentFish) {
				m_floatingFishMoveNum.push_back(1);
				dynamic_cast<FloatingFishMoveSet*>(m_currentSprite)->fish = m_floatingFish[currentFish];
				dynamic_cast<FloatingFishMoveSet*>(m_currentSprite)->distanceToFish = Vector2(m_currentSprite->GetPosition().x - m_floatingFish[currentFish]->GetPosition().x, m_currentSprite->GetPosition().y - m_floatingFish[currentFish]->GetPosition().y);
				m_fLoatingFishMove.push_back(m_currentSprite);
			}
			else {
				m_floatingFishMoveNum[currentFish]++;
				dynamic_cast<FloatingFishMoveSet*>(m_currentSprite)->fish = m_floatingFish[currentFish];
				dynamic_cast<FloatingFishMoveSet*>(m_currentSprite)->distanceToFish = Vector2(m_currentSprite->GetPosition().x - m_floatingFish[currentFish]->GetPosition().x, m_currentSprite->GetPosition().y - m_floatingFish[currentFish]->GetPosition().y);
				m_fLoatingFishMove.push_back(m_currentSprite);
			}
			m_currentSprite = NULL; Singleton<InputManager>::GetInstance()->fixButton();
		}

		return;
	}

	if (!m_floatingFishMoveNum.empty()) {
		for (int i = 0;i < m_floatingFishMoveNum.size();i++) {
			//printf("%d ", m_floatingFishMoveNum[i]);
		}
	//	printf("%d  \n", m_fLoatingFishMove.size());
	}
	int checkChoose = Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT);
	if (m_currentSprite != NULL && (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_MOVE || Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_RELEASE) && !checkChoose) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		Vector3 Pos = m_currentSprite->GetPosition();
		Pos.x = pos3D.x;
		Pos.y = pos3D.y;
		m_currentSprite->SetPosition(Pos);
		Vector2 scale = m_currentSprite->GetScale();
		scale = scale * (1 + (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::UP) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::DOWN)) * deltaTime);
		float rota = m_currentSprite->GetRotation();
		rota += (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::RIGHT) - Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LEFT)) * deltaTime;
		m_currentSprite->SetScale(scale);
		m_currentSprite->SetRotation(rota);
		for (int i = 0;i < m_activeList.size();i++) {
			Vector3 tmpPos = m_activeList[i]->GetPosition();
			tmpPos.x = Pos.x + m_distance_x[i];
			tmpPos.y = Pos.y + m_distance_y[i];
			m_activeList[i]->SetPosition(tmpPos);
		}
		if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::SPACE)) {
			if (m_currentSprite == m_victoryPoint) return;
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
		else if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::TAB)) {
			if (m_currentSprite == m_victoryPoint) return;
			Sprite* tmp = cloneSprite(m_currentSprite);
			m_currentSprite->SetColor(m_norColor, 1);
			m_currentSprite = tmp;
			Singleton<SceneManager2D>::GetInstance()->AddObject(tmp, MAP_OBJECT);
			pushSprite(tmp);
			if (!m_activeList.empty()) {
				std::vector<Sprite*> v_tmp;
				for (int i = 0;i < m_activeList.size();i++) {
					Sprite* tmp = cloneSprite(m_activeList[i]);
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
			Singleton<InputManager>::GetInstance()->KeyPressed(VK_TAB, false);


		}
		//Singleton<InputManager>::GetInstance()->fixButton();

	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK && m_currentSprite == NULL) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		m_currentSprite = checkInside(pos3D.x, pos3D.y);
		if (m_currentSprite) m_currentSprite->SetColor(m_chooseColor, 1);
		Singleton<InputManager>::GetInstance()->fixButton();
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK && m_currentSprite != NULL && !checkChoose) {
		m_currentSprite->SetColor(m_norColor, 1);
		if ((dynamic_cast<FloatingFish*>(m_currentSprite))) {
			bool check = true;
			for (int i = 0;i < m_fLoatingFishMove.size();i++) {
				if (dynamic_cast<FloatingFishMoveSet*>(m_fLoatingFishMove[i])->fish == m_currentSprite) {
					check = false;
					break;
				}

			}
			m_floatingCreated = check;
		}
		m_currentSprite = NULL; Singleton<InputManager>::GetInstance()->fixButton();

		for (int i = 0;i < m_activeList.size();i++) m_activeList[i]->SetColor(m_norColor, 1);
		m_distance_x.clear();m_distance_y.clear();
		m_activeList.clear();
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK && m_currentSprite != NULL && checkChoose) {
		int iMousePosX, iMousePosY, pre_x, pre_y;
		Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
		Vector3 pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(iMousePosX, iMousePosY, MAP_OBJECT);
		Sprite* chooseSprite = checkInside(pos3D.x, pos3D.y), * tmpSprite;

		if (chooseSprite) {
			m_activeList.push_back(m_currentSprite);
			m_currentSprite = chooseSprite;
			m_currentSprite->SetColor(m_chooseColor, 1);
			Vector2 distace(chooseSprite->GetPosition().x - m_currentSprite->GetPosition().x, chooseSprite->GetPosition().y - m_currentSprite->GetPosition().y);
			m_distance_x.push_back(distace.x);m_distance_y.push_back(distace.y);
			for (int i = 0;i < m_activeList.size();i++) {
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
		alpha = 1;
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
	else if (m_curent->type == FLOATINGFISH_UNIT) {
		int iNumOfObject, iObjectId;
		int iMaterialId;
		int iMainTexId;
		int iFontId;
		Vector3 position;
		float rotation;
		Vector2 scale;
		unsigned int uiHexColor;
		float alpha;
		FloatingFish* obs = new FloatingFish(m_curent->id);
		iMaterialId = 0;
		iMainTexId = m_curent->mainTex;
		rotation = 0;
		scale.x = 1;
		scale.y = 1;
		uiHexColor = 0xffffff;
		alpha = 1;
		position.x = 3;
		position.y = 3;
		position.z = 8;
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

		m_floatingFish.push_back(obs);
		//m_floatingCreated = true;
	}
	else if (m_curent->type == ROCKY_UNIT) {
		int iNumOfObject, iObjectId;
		int iMaterialId;
		int iMainTexId;
		int iFontId;
		Vector3 position;
		float rotation;
		Vector2 scale;
		unsigned int uiHexColor;
		float alpha;
		RockyGolem* obs = new RockyGolem(m_curent->id);
		iMaterialId = 0;
		iMainTexId = m_curent->mainTex;
		rotation = 0;
		scale.x = 1;
		scale.y = 1;
		uiHexColor = 0xffffff;
		alpha = 1;
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

		m_rockyGolem.push_back(obs);
	}
	else if (m_curent->type == BAT_UNINT) {
		int iNumOfObject, iObjectId;
		int iMaterialId;
		int iMainTexId;
		int iFontId;
		Vector3 position;
		float rotation;
		Vector2 scale;
		unsigned int uiHexColor;
		float alpha;
		VampireBat* obs = new VampireBat(-1, Vector2(0, 0), 1);;
		iMaterialId = 0;
		iMainTexId = m_curent->mainTex;
		rotation = 0;
		scale.x = 1;
		scale.y = 1;
		uiHexColor = 0xffffff;
		alpha = 1;
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

		m_vampireBat.push_back(obs);
	}
	else if (m_curent->type == DEADLYBIRD_UNIT) {
		int iNumOfObject, iObjectId;
		int iMaterialId;
		int iMainTexId;
		int iFontId;
		Vector3 position;
		float rotation;
		Vector2 scale;
		unsigned int uiHexColor;
		float alpha;
		DeadlyBird* obs = new DeadlyBird(m_curent->id);
		iMaterialId = 0;
		iMainTexId = m_curent->mainTex;
		rotation = 0;
		scale.x = 1;
		scale.y = 1;
		uiHexColor = 0xffffff;
		alpha = 1;
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

		m_deadlyBird.push_back(obs);
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
	for (int i = 0;i < m_floatingFish.size();i++) {
		if (m_floatingFish[i] == pointer) {
			for (int j = 0;j < m_fLoatingFishMove.size();j++) {
				if (dynamic_cast<FloatingFishMoveSet*>(m_fLoatingFishMove[j])->fish == pointer) {
					Sprite* tmp = m_fLoatingFishMove[j];
					Singleton<SceneManager2D>::GetInstance()->RemoveMapObject(m_fLoatingFishMove[j]);
					m_fLoatingFishMove.erase(m_fLoatingFishMove.begin() + j);
					delete tmp;
					j--;
				}
			}

			m_floatingFish.erase(m_floatingFish.begin() + i);
			m_floatingFishMoveNum.erase(m_floatingFishMoveNum.begin() + i);
			break;
		}
	}
	for (int i = 0;i < m_rockyGolem.size();i++) {
		if (m_rockyGolem[i] == pointer) {
			m_rockyGolem.erase(m_rockyGolem.begin() + i);
			break;
		}
	}
	for (int i = 0;i < m_vampireBat.size();i++) {
		if (m_vampireBat[i] == pointer) {
			m_vampireBat.erase(m_vampireBat.begin() + i);
			break;
		}
	}
	for (int i = 0;i < m_deadlyBird.size();i++) {
		if (m_deadlyBird[i] == pointer) {
			m_deadlyBird.erase(m_deadlyBird.begin() + i);
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
	for (int i = 0;i < m_floatingFish.size();i++) {
		Vector3 pos3d = m_floatingFish[i]->GetPosition();
		float width = m_floatingFish[i]->GetOrgSize().x * m_floatingFish[i]->GetScale().x;
		float height = m_floatingFish[i]->GetOrgSize().y * m_floatingFish[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_floatingFish[i];
	}
	for (int i = 0;i < m_rockyGolem.size();i++) {
		Vector3 pos3d = m_rockyGolem[i]->GetPosition();
		float width = m_rockyGolem[i]->GetOrgSize().x * m_rockyGolem[i]->GetScale().x;
		float height = m_rockyGolem[i]->GetOrgSize().y * m_rockyGolem[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_rockyGolem[i];
	}
	for (int i = 0;i < m_vampireBat.size();i++) {
		Vector3 pos3d = m_vampireBat[i]->GetPosition();
		float width = m_vampireBat[i]->GetOrgSize().x * m_vampireBat[i]->GetScale().x;
		float height = m_vampireBat[i]->GetOrgSize().y * m_vampireBat[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_vampireBat[i];
	}
	for (int i = 0;i < m_deadlyBird.size();i++) {
		Vector3 pos3d = m_deadlyBird[i]->GetPosition();
		float width = m_deadlyBird[i]->GetOrgSize().x * m_deadlyBird[i]->GetScale().x;
		float height = m_deadlyBird[i]->GetOrgSize().y * m_deadlyBird[i]->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_deadlyBird[i];
	}
	{
		Vector3 pos3d = m_victoryPoint->GetPosition();
		float width = m_victoryPoint->GetOrgSize().x * m_victoryPoint->GetScale().x;
		float height = m_victoryPoint->GetOrgSize().y * m_victoryPoint->GetScale().y;
		if (x< pos3d.x + width / 2 && x > pos3d.x - width / 2 && y< pos3d.y + height / 2 && y > pos3d.y - height / 2)
			return m_victoryPoint;
	}
	return nullptr;
}

void MapEditor::pushSprite(Sprite* pointer)
{
	if (dynamic_cast<Obstacle*>(pointer)) {
		m_Obstacle.push_back(pointer);
	}
	else if (dynamic_cast<SuicideBug*>(pointer)) {
		m_suicudeBug.push_back(pointer);
	}
	else if (dynamic_cast<FloatingFish*>(pointer)) {
		m_floatingFish.push_back(pointer);
	}
	else if (dynamic_cast<RockyGolem*>(pointer)) {
		m_rockyGolem.push_back(pointer);
	}
	else if (dynamic_cast<VampireBat*>(pointer)) {
		m_vampireBat.push_back(pointer);
	}
	else if (dynamic_cast<DeadlyBird*>(pointer)) {
		m_deadlyBird.push_back(pointer);
	}
}

Sprite* MapEditor::cloneSprite(Sprite* source)
{
	Sprite* destination = NULL;
	if (dynamic_cast<Obstacle*>(source)) {
		destination = new Obstacle(0, 0);
		destination->Init(*source);
	}
	else if (dynamic_cast<SuicideBug*>(source)) {
		destination = new SuicideBug(0);
		destination->Init(*source);
	}
	else if (dynamic_cast<FloatingFish*>(source)) {
		destination = new FloatingFish(0);
		destination->Init(*source);
		int k = 0;
		for (int j = 0;j < m_fLoatingFishMove.size();j++) {
			if (dynamic_cast<FloatingFishMoveSet*>(m_fLoatingFishMove[j])->fish == source) {
				k++;
				Sprite* tmp = createX();
				dynamic_cast<FloatingFishMoveSet*>(tmp)->fish = destination;
				dynamic_cast<FloatingFishMoveSet*>(tmp)->distanceToFish = dynamic_cast<FloatingFishMoveSet*>(m_fLoatingFishMove[j])->distanceToFish;
				m_fLoatingFishMove.push_back(tmp);
				Singleton<SceneManager2D>::GetInstance()->AddObject(tmp, MAP_OBJECT);
			}

		}
		m_floatingFishMoveNum.push_back(k);
	}
	else if (dynamic_cast<RockyGolem*>(source)) {
		destination = new RockyGolem(0);
		destination->Init(*source);
	}
	else if (dynamic_cast<VampireBat*>(source)) {
		destination = new VampireBat(-1, Vector2(0, 0), 1);
		destination->Init(*source);
	}
	else if (dynamic_cast<DeadlyBird*>(source)) {
		destination = new DeadlyBird(0);
		destination->Init(*source);
	}
	return destination;
}

FloatingFishMoveSet* MapEditor::createX()
{
	FloatingFishMoveSet* x_sprite = new FloatingFishMoveSet(0);
	int iNumOfObject, iObjectId;
	int iMaterialId;
	int iMainTexId;
	int iFontId;
	Vector3 position;
	float rotation;
	Vector2 scale;
	unsigned int uiHexColor;
	float alpha;
	iMaterialId = 0;
	iMainTexId = 55201;
	rotation = 0;
	scale.x = 0.5;
	scale.y = 0.5;
	uiHexColor = 0xffffff;
	alpha = 0.9;
	position.x = 3;
	position.y = 3;
	position.z = 9;
	x_sprite->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
	return x_sprite;
}

void MapEditor::saveFile(char* dataSceneFile)
{
	const char* resourceDir = Globals::resourceDir;
	char filePath[512];
	strcpy(filePath, resourceDir);
	strcat(filePath, dataSceneFile);
	FILE* fIn = fopen(filePath, "w");
	{fprintf(fIn, "#MAP Abyss\n");
	fprintf(fIn, "WIDTH 120\n");
	fprintf(fIn, "HEIGHTWIN %d\n",(int)m_victoryPoint->GetPosition().y);
	fprintf(fIn, "\n");
	fprintf(fIn, "#PLAYER 0\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 10101\n");
	fprintf(fIn, "POSITION 0 0 0\n");
	fprintf(fIn, "ROTATION 0\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "MISSON 1\n");
	fprintf(fIn, "\n");
	fprintf(fIn, "#BACKGROUNDS 1\n");
	fprintf(fIn, "ID 1\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 70101\n");
	fprintf(fIn, "POSITION 0 0 50\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "LOOP 1 3\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "#BORDER LEFT\n");
	fprintf(fIn, "ID 2\n");
	fprintf(fIn, "DAMAGE 1\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 70201\n");
	fprintf(fIn, "POSITION 0 0 45\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "LOOP 1 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "#BORDER RIGHT\n");
	fprintf(fIn, "ID 3\n");
	fprintf(fIn, "DAMAGE 1\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 70202\n");
	fprintf(fIn, "POSITION 0 0 45\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "LOOP 1 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "\n");
	fprintf(fIn, "#HUD\n");
	fprintf(fIn, "+ HEALTH ICON\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 60101\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR E84545 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "+ HEALTH BAR\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 60102\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR E84545 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "+ HEALTH DECORATE\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 60101\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR E84545 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "+ WEAPON ICON\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR FFFFFF 1\n");
	fprintf(fIn, "+ TARGET ICON\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR E84545 1\n");
	fprintf(fIn, "+ BULLET STATUS\n");
	fprintf(fIn, "MATERIAL 1\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR FFFFFF 1\n");
	fprintf(fIn, "FONT 2\n");
	fprintf(fIn, "+ SCORE\n");
	fprintf(fIn, "MATERIAL 1\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR FFFFFF 1\n");
	fprintf(fIn, "FONT 2\n");
	fprintf(fIn, "\n");
	fprintf(fIn, "#PLAYER_BULLET 2\n");
	fprintf(fIn, "ID 0\n");
	fprintf(fIn, "INIT 100\n");
	fprintf(fIn, "CAPACITY 200\n");
	fprintf(fIn, "MASS 1\n");
	fprintf(fIn, "GRAVITY SCALE 0\n");
	fprintf(fIn, "DAMAGE 5\n");
	fprintf(fIn, "INIT SPEED 30\n");
	fprintf(fIn, "EXIST TIME 10\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 55101\n");
	fprintf(fIn, "SCALE 0.75 0.75\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "ID 1\n");
	fprintf(fIn, "INIT 30\n");
	fprintf(fIn, "CAPACITY 100\n");
	fprintf(fIn, "MASS 1\n");
	fprintf(fIn, "GRAVITY SCALE 1\n");
	fprintf(fIn, "DAMAGE 10\n");
	fprintf(fIn, "INIT SPEED 15\n");
	fprintf(fIn, "EXIST TIME 10\n");
	fprintf(fIn, "MATERIAL 0\n");
	fprintf(fIn, "MAINTEX 55201\n");
	fprintf(fIn, "SCALE 1 1\n");
	fprintf(fIn, "COLOR ffffff 1\n");
	fprintf(fIn, "ANIMATIONS 0\n");
	fprintf(fIn, "\n");
	fprintf(fIn, "#WEAPON 7\n");
	fprintf(fIn, "ID 0\n");
	fprintf(fIn, "TYPE SIMPLEGUN\n");
	fprintf(fIn, "NAME Pistol\n");
	fprintf(fIn, "WEAPON TEX 50301\n");
	fprintf(fIn, "TARGET TEX 60201\n");
	fprintf(fIn, "BULLET TYPE 0\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.1\n");
	fprintf(fIn, "RANDOM ANGLE 5\n");
	fprintf(fIn, "FIRE AT ONCE 1\n");
	fprintf(fIn, "ID 1\n");
	fprintf(fIn, "TYPE AUTOGUN\n");
	fprintf(fIn, "NAME AK\n");
	fprintf(fIn, "WEAPON TEX 50101\n");
	fprintf(fIn, "TARGET TEX 60201\n");
	fprintf(fIn, "BULLET TYPE 0\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.75\n");
	fprintf(fIn, "RANDOM ANGLE 5\n");
	fprintf(fIn, "FIRE AT ONCE 1\n");
	fprintf(fIn, "SHORT RECHARGE TIME 0.05\n");
	fprintf(fIn, "SPREAD ANGLE 10\n");
	fprintf(fIn, "FIRE AMOUNT 5\n");
	fprintf(fIn, "ID 2\n");
	fprintf(fIn, "TYPE SIMPLEGUN\n");
	fprintf(fIn, "NAME Shotgun\n");
	fprintf(fIn, "WEAPON TEX 50501\n");
	fprintf(fIn, "TARGET TEX 60202\n");
	fprintf(fIn, "BULLET TYPE 0\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.1\n");
	fprintf(fIn, "RANDOM ANGLE 60\n");
	fprintf(fIn, "FIRE AT ONCE 5\n");
	fprintf(fIn, "ID 3\n");
	fprintf(fIn, "TYPE AUTOGUN\n");
	fprintf(fIn, "NAME Uzi\n");
	fprintf(fIn, "WEAPON TEX 50701\n");
	fprintf(fIn, "TARGET TEX 60203\n");
	fprintf(fIn, "BULLET TYPE 0\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.75\n");
	fprintf(fIn, "RANDOM ANGLE 15\n");
	fprintf(fIn, "FIRE AT ONCE 1\n");
	fprintf(fIn, "SHORT RECHARGE TIME 0.05\n");
	fprintf(fIn, "SPREAD ANGLE 30\n");
	fprintf(fIn, "FIRE AMOUNT 7\n");
	fprintf(fIn, "ID 4\n");
	fprintf(fIn, "TYPE SIMPLEGUN\n");
	fprintf(fIn, "NAME Sniper\n");
	fprintf(fIn, "WEAPON TEX 50601\n");
	fprintf(fIn, "TARGET TEX 60204\n");
	fprintf(fIn, "BULLET TYPE 0\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.1\n");
	fprintf(fIn, "RANDOM ANGLE 0\n");
	fprintf(fIn, "FIRE AT ONCE 1\n");
	fprintf(fIn, "ID 5\n");
	fprintf(fIn, "TYPE SIMPLEGUN\n");
	fprintf(fIn, "NAME Cannon\n");
	fprintf(fIn, "WEAPON TEX 50201\n");
	fprintf(fIn, "TARGET TEX 60203\n");
	fprintf(fIn, "BULLET TYPE 1\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.1\n");
	fprintf(fIn, "RANDOM ANGLE 5\n");
	fprintf(fIn, "FIRE AT ONCE 1\n");
	fprintf(fIn, "ID 6\n");
	fprintf(fIn, "TYPE SIMPLEGUN\n");
	fprintf(fIn, "NAME SuperCannon\n");
	fprintf(fIn, "WEAPON TEX 50201\n");
	fprintf(fIn, "TARGET TEX 60203\n");
	fprintf(fIn, "BULLET TYPE 1\n");
	fprintf(fIn, "OPPOSITE FORCE 0.5\n");
	fprintf(fIn, "RECHARGE TIME 0.1\n");
	fprintf(fIn, "RANDOM ANGLE 45\n");
	fprintf(fIn, "FIRE AT ONCE 3\n");
	}
	fprintf(fIn, "OBSTACLE_TYPE_0 %d\n", m_Obstacle.size());
	int currentMove = 0;
	for (int i = 0;i < m_Obstacle.size();i++) {
		fprintf(fIn, "ID %d\n", i + 1);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_Obstacle[i]->GetMainTexId());
		fprintf(fIn, "POSITION %f %f %f\n", m_Obstacle[i]->GetPosition().x, m_Obstacle[i]->GetPosition().y, m_Obstacle[i]->GetPosition().z);
		fprintf(fIn, "ROTATION %f\n", m_Obstacle[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_Obstacle[i]->GetScale().x, m_Obstacle[i]->GetScale().y);
		int r = m_Obstacle[i]->GetColor().x * 255, g = m_Obstacle[i]->GetColor().y * 255, b = m_Obstacle[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
		char type[50] = "NONE";
		int check = false;
		for (int j = 0;j < m_rectagle.size();j++) {
			if (m_rectagle[j] == m_Obstacle[i]->GetMainTexId()) {
				strcpy(type, "RECT");
				check = true;
				break;
			}
		}
		if (!check) {
			for (int j = 0;j < m_triale.size();j++) {
				if (m_triale[j] == m_Obstacle[i]->GetMainTexId()) {
					strcpy(type, "TRIANGLE");
					check = true;
					break;
				}
			}
		}
		fprintf(fIn, "TYPE %s\n", type);
		fprintf(fIn, "ANIMATIONS 0\n");
	}
	fprintf(fIn, "#ENEMY\n");
	fprintf(fIn, "#FLOATINGFISH\n");
	{
		fprintf(fIn, "ANIMATIONS 1\n");
		fprintf(fIn, "DEFAULT 0\n");
		fprintf(fIn, "ANIM 0\n");
		fprintf(fIn, "TYPE SPLIT\n");
		fprintf(fIn, "CYCLE TIME 1.5\n");
		fprintf(fIn, "TEXTURE 30102\n");
		fprintf(fIn, "SPLITXY 9 1\n");

	}
	fprintf(fIn, "COUNT %d\n", m_floatingFish.size());
	for (int i = 0;i < m_floatingFish.size();i++) {
		fprintf(fIn, "ID %d\n", i);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_floatingFish[i]->GetMainTexId());
		fprintf(fIn, "NUM OFTARGET %d", m_floatingFishMoveNum[i]);
		for (int j = 0;j < m_floatingFishMoveNum[i];j++) {
			fprintf(fIn, "TARGET %f %f\n", m_fLoatingFishMove[currentMove]->GetPosition().x, m_fLoatingFishMove[currentMove]->GetPosition().y);
			++currentMove;
		}
		fprintf(fIn, "POSITION %f %f %f\n", m_floatingFish[i]->GetPosition().x, m_floatingFish[i]->GetPosition().y, m_floatingFish[i]->GetPosition().z);
		fprintf(fIn, "ROTATION %f\n", m_floatingFish[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_floatingFish[i]->GetScale().x, m_floatingFish[i]->GetScale().y);
		int r = m_floatingFish[i]->GetColor().x * 255, g = m_floatingFish[i]->GetColor().y * 255, b = m_floatingFish[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));

	}
	fprintf(fIn, "#SUICIDEBUG\n");
	{
		fprintf(fIn, "ANIMATIONS 1\n");
		fprintf(fIn, "DEFAULT 0\n");
		fprintf(fIn, "ANIM 0\n");
		fprintf(fIn, "TYPE SPLIT\n");
		fprintf(fIn, "CYCLE TIME 0.24\n");
		fprintf(fIn, "TEXTURE 30203\n");
		fprintf(fIn, "SPLITXY 12 1\n");}
	fprintf(fIn, "COUNT %d\n", m_suicudeBug.size());
	for (int i = 0;i < m_suicudeBug.size();i++) {
		fprintf(fIn, "ID %d\n", i);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_suicudeBug[i]->GetMainTexId());
		fprintf(fIn, "POSITION %f %f %f\n", m_suicudeBug[i]->GetPosition().x, m_suicudeBug[i]->GetPosition().y, m_suicudeBug[i]->GetPosition().z);
		fprintf(fIn, "ROTATION %f\n", m_suicudeBug[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_suicudeBug[i]->GetScale().x, m_suicudeBug[i]->GetScale().y);
		int r = m_suicudeBug[i]->GetColor().x * 255, g = m_suicudeBug[i]->GetColor().y * 255, b = m_suicudeBug[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
	}
	fprintf(fIn, "#ROCKYGOLEM\n");
	fprintf(fIn, "ANIMATIONS 0\n");

	fprintf(fIn, "COUNT %d\n", m_rockyGolem.size());
	for (int i = 0;i < m_rockyGolem.size();i++) {
		fprintf(fIn, "ID %d\n", i);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_rockyGolem[i]->GetMainTexId());
		fprintf(fIn, "POSITION %f %f %f\n", m_rockyGolem[i]->GetPosition().x, m_rockyGolem[i]->GetPosition().y, m_rockyGolem[i]->GetPosition().z);
		fprintf(fIn, "ROTATION %f\n", m_rockyGolem[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_rockyGolem[i]->GetScale().x, m_rockyGolem[i]->GetScale().y);
		int r = m_rockyGolem[i]->GetColor().x * 255, g = m_rockyGolem[i]->GetColor().y * 255, b = m_rockyGolem[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
	}

	fprintf(fIn, "#DEADLYBIRD\n");
	{
		fprintf(fIn, "ANIMATIONS 1\n");
		fprintf(fIn, "DEFAULT 0\n");
		fprintf(fIn, "ANIM 0\n");
		fprintf(fIn, "TYPE SPLIT\n");
		fprintf(fIn, "CYCLE TIME 0.6\n");
		fprintf(fIn, "TEXTURE 30503\n");
		fprintf(fIn, "SPLITXY 17 1\n"); }
	fprintf(fIn, "COUNT %d\n", m_deadlyBird.size());
	for (int i = 0;i < m_deadlyBird.size();i++) {
		fprintf(fIn, "ID %d\n", i);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_deadlyBird[i]->GetMainTexId());
		fprintf(fIn, "POSITION %f %f %f\n", m_deadlyBird[i]->GetPosition().x, m_deadlyBird[i]->GetPosition().y, m_deadlyBird[i]->GetPosition().z);
		fprintf(fIn, "ROTATION %f\n", m_deadlyBird[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_deadlyBird[i]->GetScale().x, m_deadlyBird[i]->GetScale().y);
		int r = m_deadlyBird[i]->GetColor().x * 255, g = m_deadlyBird[i]->GetColor().y * 255, b = m_deadlyBird[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
	}

	fprintf(fIn, "#VAMPIREBAT\n");
	fprintf(fIn, "ANIMATIONS 1\n");
	fprintf(fIn, "DEFAULT 0\n");
	fprintf(fIn, "ANIM 0\n");
	fprintf(fIn, "TYPE SPLIT\n");
	fprintf(fIn, "CYCLE TIME 0.6\n");
	fprintf(fIn, "TEXTURE 30602\n");
	fprintf(fIn, "SPLITXY 10 1\n");

	fprintf(fIn, "COUNT %d", m_vampireBat.size());
	for (int i = 0;i < m_vampireBat.size();i++) {
		fprintf(fIn, "ID %d\n", i);
		fprintf(fIn, "MATERIAL 0\n");
		fprintf(fIn, "MAINTEX %d\n", m_vampireBat[i]->GetMainTexId());
		fprintf(fIn, "CENTER POSITION %f %f %f\n", m_vampireBat[i]->GetPosition().x, m_vampireBat[i]->GetPosition().y, m_vampireBat[i]->GetPosition().z);
		fprintf(fIn, "RANGE 4\n");
		fprintf(fIn, "ROTATION %f\n", m_vampireBat[i]->GetRotation() * 180 / M_PI);
		fprintf(fIn, "SCALE %f %f\n", m_vampireBat[i]->GetScale().x, m_vampireBat[i]->GetScale().y);
		int r = m_vampireBat[i]->GetColor().x * 255, g = m_vampireBat[i]->GetColor().y * 255, b = m_vampireBat[i]->GetColor().z * 255;
		fprintf(fIn, "COLOR %x 1\n", ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
	}

	fprintf(fIn, "#CAMERA\n");
	fprintf(fIn, "NEAR 0.1\n");
	fprintf(fIn, "FAR 120\n");
	fprintf(fIn, "ZOOM 40\n");
	fprintf(fIn, "POSITION 0 0 -20\n");
	fprintf(fIn, "DUTCH \n");


	fclose(fIn);
}

void MapEditor::loadAnimation(FILE* fIn)
{
	int iNumOfAnimations;
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	if (iNumOfAnimations > 0) {
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
			fscanf(fIn, "ANIM %d\n", &iAnimStateId);
			fscanf(fIn, "TYPE %s\n", animType);
			fscanf(fIn, "CYCLE TIME %f\n", &cycleTime);

			if (strcmp("SPLIT", animType) == 0) {
				fscanf(fIn, "TEXTURE %d\n", &iAnimTexId);
				fscanf(fIn, "SPLITXY %d %d\n", &iSplitX, &iSplitY);
				iCountValidAnim++;
			}
			else if (strcmp("MULTITEX", animType) == 0) {
				//TEXTURES 7: 50101 50201 50301 50401 50501 50601 50701
				fscanf(fIn, "TEXTURES %d:", &iNumOfAnimTextures);
				for (int j = 0;j < iNumOfAnimTextures;j++) {
					fscanf(fIn, " *%d");
				}
				fscanf(fIn, "\n");
				iCountValidAnim++;
			}
			else {
			}
		}
	}
}

void MapEditor::pushBackBoxType(int texture, int type)
{
	if (type == REACT_UNIT) {
		m_rectagle.push_back(texture);
	}
	else if (type == TRIAGLE_UNIT) {
		m_triale.push_back(texture);
	}
}

FloatingFishMoveSet::FloatingFishMoveSet(int id) :Sprite(id)
{
}
