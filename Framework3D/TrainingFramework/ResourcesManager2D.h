#pragma once
#include "stdafx.h"
#include "Model3D.h"
#include "Texture.h"
#include "Shaders.h"
#include "Material2D.h"
#include <vector>

class ResourceManager2D {

public:
	ResourceManager2D();
	~ResourceManager2D();
	
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