#pragma once
#include "ResourcesManager2D.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "MaterialPostProcess.h"
#include "MaterialPPColorCorrection.h"
#include "Singleton.h"
#include <vector>

class PostProcessingManager
{
private:
	GLuint m_frameBufObjColorCorrection, m_depthBufObjColorCorrection;
	GLuint m_textureHandleColorCorrection;
	bool m_isEnabledColorCorrection, m_canRunColorCorrection;

	GLuint m_frameBufObj, m_depthBufObj;
	GLuint m_textureHandle;
	bool m_isEnabled, m_canRun;
	MaterialPostProcess* m_material;
	MaterialPPColorCorrection* m_materialColorCorrection;
	Model3D* m_model;

	std::vector<Shaders*> m_listShader;
	std::vector<MaterialPostProcess*> m_listMaterial;
	Shaders* GetShader(int iShaderId);
	MaterialPostProcess* GetMaterial(int iMaterialId);
public:
	Vector4 m_colorOffset, m_colorMul;
public:
	PostProcessingManager();
	~PostProcessingManager();
	void Load(char* postProcessFilePath);
	void SetMaterial(int iMaterialId);
	void Prepare();
	void Render();
};

