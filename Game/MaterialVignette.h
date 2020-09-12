#pragma once
#include "MaterialPostProcess.h"
class MaterialVignette : public MaterialPostProcess
{
private:
	GLuint m_uIntensifyLocation;
	GLuint m_uExtendLocation;
	GLuint m_uColorLocation;
public:
	float m_uIntensify, m_uExtend;
	Vector4 m_uColor;
public:
	MaterialVignette(int id);
	~MaterialVignette();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(GLuint textureHandle);
};

