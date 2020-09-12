#include "MaterialVignette.h"



MaterialVignette::MaterialVignette(int id) : MaterialPostProcess(id)
{
	m_uIntensify = 15.0;
	m_uExtend = 0.25;
	m_uColor = Vector4(0,0,0,1);
}

MaterialVignette::~MaterialVignette()
{
}

bool MaterialVignette::Init(Shaders* shader)
{
	bool isSuccess = MaterialPostProcess::Init(shader);
	if (!isSuccess) return false;
	m_uIntensifyLocation = glGetUniformLocation(m_shader->program, "u_intensify");
	m_uExtendLocation = glGetUniformLocation(m_shader->program, "u_extend");
	m_uColorLocation = glGetUniformLocation(m_shader->program, "u_color");
	return true;
}

void MaterialVignette::PrepareShader(GLuint textureHandle)
{
	MaterialPostProcess::PrepareShader(textureHandle);
	if (m_uIntensifyLocation != -1) {
		glUniform1f(m_uIntensifyLocation, (GLfloat)m_uIntensify);
	}
	if (m_uExtendLocation != -1) {
		glUniform1f(m_uExtendLocation, (GLfloat)m_uExtend);
	}
	if (m_uColorLocation != -1) {
		glUniform4fv(m_uColorLocation, 1, (GLfloat*)& m_uColor);
	}
}
