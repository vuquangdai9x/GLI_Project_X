#include "DecorateObjectManager.h"



DecorateObjectManager::DecorateObjectManager()
{
	m_iObjIdCount = 100000;
}

DecorateObjectManager::~DecorateObjectManager()
{
	for (int i = 0; i < m_listTemplateObject.size(); i++) {
		delete m_listTemplateObject[i];
	}
	m_listTemplateObject.clear();
}

bool DecorateObjectManager::LoadAnimation(FILE* fIn, DecorateObject* sprite) {
	int iNumOfAnimations;
	fscanf(fIn, "ANIMATIONS %d\n", &iNumOfAnimations);
	if (iNumOfAnimations > 0) {
		AnimationController& animCtrl = sprite->GetAnimationController();
		int iDefaultAnimId;
		fscanf(fIn, "DEFAULT %d\n", &iDefaultAnimId);
		int iCountValidAnim = 0;
		for (int i = 0; i < iNumOfAnimations; i++) {
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
				for (int j = 0; j < iNumOfAnimTextures; j++) {
					fscanf(fIn, " %d", &(listTextAnim[j]));
				}
				fscanf(fIn, "\n");
				animCtrl.AddAnimState(iAnimStateId, iNumOfAnimTextures, listTextAnim, cycleTime);
				delete[] listTextAnim;
				iCountValidAnim++;
			}
			else {
				printf("[ERR] DecorateObjectManager:LoadAnimation: AnimId %d: type invalid: %s", iAnimStateId, animType);
			}
		}

		if (iCountValidAnim > 0) {
			//printf("[msg] DecorateObjectManager:LoadAnimation: Loaded %d animation\n", iCountValidAnim);
			animCtrl.SetDefaultAnimState(iDefaultAnimId);
			sprite->SetUseAnimation(true);
			return true;
		}
		else {
			printf("[ERR] DecorateObjectManager:LoadAnimation: No animation valid\n");
			return false;
		}
	}
	return false;
}

void DecorateObjectManager::Load(char * decorateObjFilePath)
{
	const char* resourceDir = Globals::resourceDir;

	// NOTE: length of file path can be not enough and cause error
	char fileName[256];
	char filePath[512];

	strcpy(filePath, resourceDir);
	strcat(filePath, decorateObjFilePath);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("[ERR] DecorateObjectManager::Load: Fails to load decorate object file\n");
		return;
	}

	int iNumOfObj;
	// NOTE: length of file name can be not enough and cause error
	char vertexShaderFile[256], fragmentShaderFile[256];
	fscanf(fIn, "\n#Decorate Objects: %d\n", &iNumOfObj);

	for (int i = 0; i < iNumOfObj; i++) {
		int iObjectId, iMaterialId, iMainTexId;
		Vector3 position;
		Vector2 scale;
		float rotation;
		unsigned int uiHexColor;
		float alpha;
		Vector2 colliderRatio;
		int iDamageEffectId;
		int iDeleteOnDestroy;
		Vector2 pivot;

		fscanf(fIn, "\nID %d\n", &iObjectId);
		fscanf(fIn, "MATERIAL %d\n", &iMaterialId);
		fscanf(fIn, "MAINTEX %d\n", &iMainTexId);
		fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
		fscanf(fIn, "ROTATION %f\n", &rotation);
		rotation = rotation * 2 * M_PI / 360;
		fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
		fscanf(fIn, "COLOR %x %f\n", &uiHexColor, &alpha);
		fscanf(fIn, "COLLIDER RATIO %f %f\n", &colliderRatio.x, &colliderRatio.y);
		fscanf(fIn, "DAMAGE EFFECT %d\n", &iDamageEffectId);
		fscanf(fIn, "DELETE ON DESTROY %d\n", &iDeleteOnDestroy);
		fscanf(fIn, "PIVOT %f %f\n", &pivot.x, &pivot.y);
		DecorateObject* obj = new DecorateObject(iObjectId);
		obj->Init(position, rotation, scale, uiHexColor, alpha, iMaterialId, iMainTexId);
		obj->InitInfo(pivot, (iDeleteOnDestroy == 1), iDamageEffectId, colliderRatio);
		LoadAnimation(fIn, obj);

		AddTemplateObject(obj);

		printf("[msg] DecorateObjectManager::Load: Loaded decorate object %d\n",iObjectId);
	}

	fclose(fIn);
}

void DecorateObjectManager::AddTemplateObject(DecorateObject * obj)
{
	if (obj != NULL)
		m_listTemplateObject.push_back(obj);
}

DecorateObject* DecorateObjectManager::CreateObject(int iObjId, Vector2 position)
{
	for (int i = 0; i < m_listTemplateObject.size(); i++) {
		if (iObjId == m_listTemplateObject[i]->GetId()) {
			m_iObjIdCount++;
			DecorateObject* obj = m_listTemplateObject[i]->CreateClone(m_iObjIdCount);
			Vector3 placePos(position.x, position.y, m_listTemplateObject[i]->GetPosition().z);
			obj->SetPosition(placePos);
			return obj;
		}
	}
}

DecorateObject * DecorateObjectManager::CreateRandomObject(Vector2 position)
{
	int objIndex = rand() % m_listTemplateObject.size();
	m_iObjIdCount++;
	DecorateObject* obj = m_listTemplateObject[objIndex]->CreateClone(m_iObjIdCount);
	Vector3 placePos(position.x, position.y, m_listTemplateObject[objIndex]->GetPosition().z);
	obj->SetPosition(placePos);
	return obj;
}
