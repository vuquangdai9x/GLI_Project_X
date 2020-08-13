#include "stdafx.h"
#include "ResourcesManager2D.h"
#include "../Framework3D/TrainingFramework/MaterialTerrain.h"
#include "../Framework3D/TrainingFramework/MaterialFire.h"
#include "../Framework3D/TrainingFramework/Material2D.h"
#include "../Framework3D/TrainingFramework/Globals.h"

ResourceManager2D::ResourceManager2D()
{
}

ResourceManager2D::~ResourceManager2D()
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

void ResourceManager2D::LoadResources(char* dataResourceFile)
{
	const char* resourceDir = Globals::resourceDir;

	// NOTE: length of file path can be not enough and cause error
	char filePath[512];

	strcpy(filePath, resourceDir);
	strcat(filePath, dataResourceFile);
	FILE* fIn = fopen(filePath, "r");
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
		Model3D* model;
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
		Texture* m_texture;
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

		if (m_texture->LoadTextureFromFile(filePath, tilingMode)) {
			m_aTexture.push_back(m_texture);
			printf("[msg] ResourceManager: Loaded Texture %d : %s . %s %d\n", iTextureId, fileName, tilingSetting, tilingMode);
		}
		else {
			printf("[ERR] ResourceManager: Failed to load Texture %s\n", fileName);
			delete m_texture;
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
		Shaders* shader;
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
			Material2D* pMaterial;
			int iShaderId;
			pMaterial = new Material2D(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			if (pMaterial->Init(iShaderId)) {
				m_aMaterial.push_back(pMaterial);
				printf("[msg] ResourceManager: Loaded Default Material %d : Shader = %d\n", iMaterialId, iShaderId);
			}
			else {
				printf("[ERR] ResourceManager: Failed to load Default Material %d : Shader = %d\n", iMaterialId, iShaderId);
				delete pMaterial;
			}
		}else {
			printf("[ERR] ResourceManager: There no material has this type\n");
		}
	}
	fclose(fIn);
}

Model3D* ResourceManager2D::GetModel(int id)
{
	for (int i = 0; i < m_aModel.size(); i++) {
		if (m_aModel[i]->m_id == id)
			return m_aModel[i];
	}
	return NULL;
}

Texture* ResourceManager2D::GetTexture(int id)
{
	for (int i = 0; i < m_aTexture.size(); i++) {
		if (m_aTexture[i]->GetId() == id)
			return m_aTexture[i];
	}
	return NULL;
}

Shaders* ResourceManager2D::GetShader(int id)
{
	for (int i = 0; i < m_aShader.size(); i++) {
		if (m_aShader[i]->m_id == id)
			return m_aShader[i];
	}
	return NULL;
}

Material2D* ResourceManager2D::GetMaterial(int id)
{
	for (int i = 0; i < m_aMaterial.size(); i++) {
		if (m_aMaterial[i]->GetId() == id)
			return m_aMaterial[i];
	}
	return NULL;
}
