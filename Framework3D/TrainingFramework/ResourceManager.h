#pragma once
#include "stdafx.h"
#include "Model3D.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material.h"
#include <vector>

class ResourceManager {
protected:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager &) {}
	ResourceManager& operator =(const ResourceManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == nullptr)
			ms_pInstance = new ResourceManager;
	}
	static ResourceManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = nullptr;
		}
	}

protected:
	static ResourceManager * ms_pInstance;

private:
	std::vector<Model3D*> m_aModel;
	std::vector<Texture*> m_aTexture;
	std::vector<Shaders*> m_aShader;
	std::vector<Material*> m_aMaterial;
public:
	void LoadResources(char * dataResourceFile);
	Model3D * GetModel(int id);
	Texture * GetTexture(int id);
	Shaders * GetShader(int id);
	Material * GetMaterial(int id);
};