#pragma once
#include "stdafx.h"
#include "Model3D.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material2D.h"
#include <vector>

class ResourceManager2D {
protected:
	ResourceManager2D();
	~ResourceManager2D();
	ResourceManager2D(const ResourceManager2D&) {}
	ResourceManager2D& operator =(const ResourceManager2D&) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == nullptr)
			ms_pInstance = new ResourceManager2D;
	}
	static ResourceManager2D* GetInstance()
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
	static ResourceManager2D* ms_pInstance;

private:
	std::vector<Model3D*> m_aModel;
	std::vector<Texture*> m_aTexture;
	std::vector<Shaders*> m_aShader;
	std::vector<Material2D*> m_aMaterial;
public:
	void LoadResources(char* dataResourceFile);
	Model3D* GetModel(int id);
	Texture* GetTexture(int id);
	Shaders* GetShader(int id);
	Material2D* GetMaterial(int id);
};