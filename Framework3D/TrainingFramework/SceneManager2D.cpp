#include "stdafx.h"
#include "SceneManager2D.h"
#include <math.h>
#include "Globals.h"
#include "Sprite.h"

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

	int iNumOfAnimations;
	int iAnimId;
	char animType[10];

	int iAnimTextureId;
	int iDivisionX, iDivisionY;
	float animDuration;

	int iNumOfAnimTexs;
	int* aiAnimTexId;

	fscanf(fIn, "#Objects: %d\n", &iNumOfObject);
	for (int i = 0; i < iNumOfObject; i++) {
		obj = new Sprite(iObjectId);
		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * 3.1416 / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		obj->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);

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

		AddObject(obj);

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
	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->Update(frameTime);
	}
	m_mainCamera->Update(frameTime);
}
void SceneManager2D::Render() {

	for (int i = 0; i < m_listObject.size(); i++) {
		m_listObject[i]->Render(m_mainCamera);
	}
}
void SceneManager2D::AddObject(Sprite* object) {
	if (m_listObject.size() == 0) m_listObject.push_back(object);
	else {
		for (int i = 0;i < m_listObject.size();i++) {
			if (object->GetPosition().z >= m_listObject[i]->GetPosition().z) {
				m_listObject.insert(m_listObject.begin() + i, object);
				break;
			}
		}
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
