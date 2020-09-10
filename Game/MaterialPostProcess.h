#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class MaterialPostProcess
{
protected:
	int m_id;
	Shaders* m_shader;

	GLint m_u_mainTextureLocation = -1;
	GLint m_a_positionLocation = -1;
	GLint m_a_uvLocation = -1;
public:
	MaterialPostProcess(int id);
	int GetId();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(GLuint textureHandle);
};

