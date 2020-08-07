#include "stdafx.h"
#include "ResourceManager.h"
#include "MaterialTerrain.h"
#include "MaterialFire.h"
#include "Globals.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// delete resources
	for (int i = 0; i < m_aModel.size(); i++) {
		delete m_aModel[i];
	}
	for (int i = 0; i < m_aTexture.size(); i++) {
		delete m_aTexture[i];
	}
	for (int i = 0; i < m_aShader.size(); i++) {
		delete m_aShader[i];
	}
	for (int i = 0; i < m_aMaterial.size(); i++) {
		delete m_aMaterial[i];
	}
}

void ResourceManager::LoadResources(char * dataResourceFile)
{
	const char *resourceDir = Globals::resourceDir;

	// NOTE: length of file path can be not enough and cause error
	char filePath[512];

	strcpy(filePath, resourceDir);
	strcat(filePath, dataResourceFile);
	FILE * fIn = fopen(filePath,"r");
	if (fIn == nullptr) {
		printf("Fails to load list resources file");
		return;
	}

	//
	// Load Models
	//

	int iNumOfModels;
	// NOTE: length of file name can be not enough and cause error
	char fileName[256];
	fscanf(fIn, "#Models: %d\n", &iNumOfModels);
	for (int i = 0; i < iNumOfModels; i++) {
		Model3D * model;
		int iModelId;
		fscanf(fIn, "ID %d\n", &iModelId);
		model = new Model3D(iModelId);

		fscanf(fIn, "FILE \"%[^\"]\"\n", fileName);
		strcpy(filePath, resourceDir);
		strcat(filePath, fileName);

		bool isLoadSuccessful = true;

		char* ext = strrchr(fileName, '.');
		if (strcmp(".nfg", ext) == 0) {
			isLoadSuccessful = model->LoadFromFile(filePath);
		}
		else if (strcmp(".raw", ext) == 0) {
			int iWidth, iHeight;
			float scale;
			fscanf(fIn, "SIZE %d %d\n", &iWidth, &iHeight);
			fscanf(fIn, "SCALE %f\n", &scale);
			isLoadSuccessful = model->LoadFromRaw(filePath, iWidth, iHeight, scale);
		}
		else {
			printf("[ERR] ResourceManager: This file extension is not supported: %s\n", ext);
			isLoadSuccessful = false;
		}

		if (isLoadSuccessful) {
			m_aModel.push_back(model);
			printf("[msg] ResourceManager: Loaded Model %d : %s\n", iModelId, fileName);
		}
		else {
			printf("[ERR] ResourceManager: Failed to load model %s\n", fileName);
			delete model;
		}
	}

	//
	// Load Textures
	//

	int iNumOfTextures;
	fscanf(fIn, "\n#2D Textures: %d\n", &iNumOfTextures);
	for (int i = 0; i < iNumOfTextures; i++) {
		Texture * m_texture;
		int iTextureId;
		char tilingSetting[10];
		Texture::TILING_MODE tilingMode;

		fscanf(fIn, "ID %d\n", &iTextureId);
		m_texture = new Texture(iTextureId, false);

		fscanf(fIn, "FILE \"%[^\"]\"\n", fileName);
		strcpy(filePath, resourceDir);
		strcat(filePath, fileName);

		fscanf(fIn, "TILING %s\n", tilingSetting);
		if (strcmp("CLAMP", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::CLAMP;
		else if (strcmp("REPEAT", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::REPEAT;
		else if (strcmp("MIRROR", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::MIRROR;
		else tilingMode = Texture::TILING_MODE::CLAMP;

		if (m_texture->LoadTextureFromFile(filePath,tilingMode)) {
			m_aTexture.push_back(m_texture);
			printf("[msg] ResourceManager: Loaded Texture %d : %s . %s %d\n", iTextureId, fileName, tilingSetting, tilingMode);
		}
		else {
			printf("[ERR] ResourceManager: Failed to load Texture %s\n", fileName);
			delete m_texture;
		}
	}

	//
	// Load Cube Textures
	//

	int iNumOfCubeTextures;
	fscanf(fIn, "\n#Cube Textures: %d\n", &iNumOfCubeTextures);
	for (int i = 0; i < iNumOfCubeTextures; i++) {
		Texture * cubeTexture;
		int iCubeTextureId;
		char cubeTextureResourceMode[10];
		char tilingSetting[10];
		Texture::TILING_MODE tilingMode;

		fscanf(fIn, "ID %d %s\n", &iCubeTextureId, cubeTextureResourceMode);
		cubeTexture = new Texture(iCubeTextureId, true);
		if (strcmp("SPLITTED", cubeTextureResourceMode) == 0) {
			int iPlaneIndex;
			char * splittedFilesName[6];

			for (int j = 0; j < 6; j++) splittedFilesName[j] = new char[256];
			for (int j = 0; j < 6; j++) {
				fscanf(fIn, "FILE %d \"%[^\"]\"\n", &iPlaneIndex, fileName);
				strcpy(splittedFilesName[iPlaneIndex], resourceDir);
				strcat(splittedFilesName[iPlaneIndex], fileName);
			}

			fscanf(fIn, "TILING %s\n", tilingSetting);
			if (strcmp("CLAMP", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::CLAMP;
			else if (strcmp("REPEAT", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::REPEAT;
			else if (strcmp("MIRROR", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::MIRROR;
			else tilingMode = Texture::TILING_MODE::CLAMP;

			if (cubeTexture->LoadCubeTextureFromSplittedFiles(splittedFilesName, tilingMode)) {
				m_aTexture.push_back(cubeTexture);
				printf("[msg] ResourceManager: Loaded Cube Texture %d . Tiling = %s %d\n", iCubeTextureId, tilingSetting, tilingMode);
				for (int j = 0; j < 6; j++) printf("\t%s\n", splittedFilesName[j]);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Cube Texture\n");
				for (int j = 0; j < 6; j++) printf("\t%s\n", splittedFilesName[j]);
				delete cubeTexture;
			}
			for (int j = 0; j<6; j++) delete [] splittedFilesName[j];
		} else if (strcmp("ONE", cubeTextureResourceMode) == 0) {
			fscanf(fIn, "FILE \"%[^\"]\"\n", fileName);
			strcpy(filePath, resourceDir);
			strcat(filePath, fileName);

			fscanf(fIn, "TILING %s\n", tilingSetting);
			if (strcmp("CLAMP", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::CLAMP;
			else if (strcmp("REPEAT", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::REPEAT;
			else if (strcmp("MIRROR", tilingSetting) == 0) tilingMode = Texture::TILING_MODE::MIRROR;
			else tilingMode = Texture::TILING_MODE::CLAMP;

			if (cubeTexture->LoadCubeTextureFromFile(filePath, tilingMode)) {
				m_aTexture.push_back(cubeTexture);
				printf("[msg] ResourceManager: Loaded Cube Texture %d: %s . Tiling = %s %d\n", iCubeTextureId, fileName, tilingSetting, tilingMode);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Cube Texture: %s\n",fileName);
				delete cubeTexture;
			}
		} else {
			fscanf(fIn, "FILE \"%[^\"]\"\n", fileName);
			printf("[ERR] ResourceManager: Can not load texture this type: \n", fileName);
		}
	}

	//
	// Load Shaders
	//
	
	int iNumOfShaders;
	// NOTE: length of file name can be not enough and cause error
	char vertexShaderFile[256], fragmentShaderFile[256];
	fscanf(fIn, "\n#Shaders: %d\n", &iNumOfShaders);
	for (int i = 0; i < iNumOfShaders; i++) {
		Shaders * shader;
		char shaderType[20];
		int iShaderId, iNumOfState;
		char stateSetting[20];

		fscanf(fIn, "ID %d\n", &iShaderId);

		fscanf(fIn, "VS: \"%[^\"]\"\n", fileName);
		strcpy(vertexShaderFile, resourceDir);
		strcat(vertexShaderFile, fileName);
		fscanf(fIn, "FS: \"%[^\"]\"\n", fileName);
		strcpy(fragmentShaderFile, resourceDir);
		strcat(fragmentShaderFile, fileName);

		fscanf(fIn, "STATES %d\n", &iNumOfState);
		for (int j = 0; j < iNumOfState; j++) {
			fscanf(fIn, "STATE %s\n", stateSetting);
		}
		shader = new Shaders(iShaderId);
		if (shader->Init(vertexShaderFile, fragmentShaderFile)) {
			m_aShader.push_back(shader);
			printf("[msg] ResourceManager: Loaded Shader %d :\n\tVS: %s\n\tFS: %s\n", iShaderId, vertexShaderFile, fragmentShaderFile);
		}
		else {
			printf("[ERR] ResourceManager: Failed to load Shader \n\tVS: %s\n\tFS: %s\n", vertexShaderFile, fragmentShaderFile);
			delete shader;
		}
	}

	//
	// Load Materials
	//
	
	int iNumOfMaterial, iMaterialId;
	char materialType[20];
	fscanf(fIn, "\n#Materials: %d\n", &iNumOfMaterial);
	for (int i = 0; i < iNumOfMaterial; i++) {
		fscanf(fIn, "ID %d %s\n", &iMaterialId, materialType);
		if (strcmp("DEFAULT", materialType) == 0) {
			Material * pMaterial;
			int iShaderId;
			pMaterial = new Material(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			if (pMaterial->Init(iShaderId)) {
				m_aMaterial.push_back(pMaterial);
				printf("[msg] ResourceManager: Loaded Default Material %d : Shader = %d\n", iMaterialId, iShaderId);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Default Material %d : Shader = %d\n", iMaterialId, iShaderId);
				delete pMaterial;
			}
		}
		else if (strcmp("TERRAIN", materialType) == 0) {
			MaterialTerrain * pMaterialTerrain;
			int iShaderId, iBlendTexId, iTex1Id, iTex2Id, iTex3Id;
			pMaterialTerrain = new MaterialTerrain(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			fscanf(fIn, "BLENDTEX %d\n", &iBlendTexId);
			fscanf(fIn, "TEXTURES %d %d %d\n", &iTex1Id, &iTex2Id, &iTex3Id);
			if (pMaterialTerrain->Init(iShaderId, iBlendTexId, iTex1Id, iTex2Id, iTex3Id)) {
				m_aMaterial.push_back(pMaterialTerrain);
				printf("[msg] ResourceManager: Loaded Terrain Material %d\n", iMaterialId);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Terrain Material %d\n", iMaterialId);
				delete pMaterialTerrain;
				pMaterialTerrain = NULL;
			}
		}
		else if (strcmp("FIRE", materialType) == 0) {
			MaterialFire* pMaterialFire;
			int iShaderId, iDisplTexId, iAlphaMaskTexId;
			float displMax, timeScale;
			pMaterialFire = new MaterialFire(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			fscanf(fIn, "DISPLTEX %d\n", &iDisplTexId);
			fscanf(fIn, "ALPHAMASK %d\n", &iAlphaMaskTexId);
			fscanf(fIn, "DISPLMAX %f\n", &displMax);
			fscanf(fIn, "TIMESCALE %f\n", &timeScale);
			if (pMaterialFire->Init(iShaderId, iDisplTexId, iAlphaMaskTexId, displMax, timeScale)) {
				m_aMaterial.push_back(pMaterialFire);
				printf("[msg] ResourceManager: Loaded Fire Material %d\n", iMaterialId);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Fire Material %d\n", iMaterialId);
				delete pMaterialFire;
				pMaterialFire = NULL;
			}
		}
		else {
			printf("[ERR] ResourceManager: There no material has this type\n");
		}
	}
	fclose(fIn);
}

Model3D * ResourceManager::GetModel(int id)
{
	for (int i = 0; i < m_aModel.size(); i++) {
		if (m_aModel[i]->m_id == id)
			return m_aModel[i];
	}
	return NULL;
}

Texture * ResourceManager::GetTexture(int id)
{
	for (int i = 0; i < m_aTexture.size(); i++) {
		if (m_aTexture[i]->GetId() == id)
			return m_aTexture[i];
	}
	return NULL;
}

Shaders * ResourceManager::GetShader(int id)
{
	for (int i = 0; i < m_aShader.size(); i++) {
		if (m_aShader[i]->m_id == id)
			return m_aShader[i];
	}
	return NULL;
}

Material * ResourceManager::GetMaterial(int id)
{
	for (int i = 0; i < m_aMaterial.size(); i++) {
		if (m_aMaterial[i]->GetId() == id)
			return m_aMaterial[i];
	}
	return NULL;
}

ResourceManager * ResourceManager::ms_pInstance = NULL;