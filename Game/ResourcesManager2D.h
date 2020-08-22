#pragma once
#pragma warning(disable: 4996)
#include "stdafx.h"
#include "../Framework3D/TrainingFramework/Model3D.h"
#include "../Framework3D/TrainingFramework/Texture.h"
#include "../Framework3D/TrainingFramework/Shaders.h"
#include "../Framework3D/TrainingFramework/Material2D.h"
#include <vector>
#include "Singleton.h"
#include "FontFace.h"

class ResourceManager2D {
public:
	ResourceManager2D();
	~ResourceManager2D();
	
private:
	std::vector<Model3D*> m_aModel;
	std::vector<Texture*> m_aTexture;
	std::vector<Shaders*> m_aShader;
	std::vector<Material2D*> m_aMaterial;
	std::vector<FontFace*> m_aFontFace;
public:
	void LoadResources(char* dataResourceFile);
	Model3D* GetModel(int id);
	Texture* GetTexture(int id);
	Shaders* GetShader(int id);
	Material2D* GetMaterial(int id);
	FontFace* GetFontFace(int id);
};