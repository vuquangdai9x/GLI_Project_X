#include "MaterialPostProcess.h"

MaterialPostProcess::MaterialPostProcess(int id): m_id(id)
{
}

int MaterialPostProcess::GetId()
{
	return m_id;
}

bool MaterialPostProcess::Init(Shaders * shader)
{
	m_shader = shader;
	
	m_u_mainTextureLocation = glGetUniformLocation(m_shader->program, "u_mainTexture");
	m_a_positionLocation = glGetAttribLocation(m_shader->program, "a_posL");
	m_a_uvLocation = glGetAttribLocation(m_shader->program, "a_uv");
	
	return true;
}

void MaterialPostProcess::PrepareShader(GLuint textureHandle)
{
	glUseProgram(m_shader->program);
	if (m_a_positionLocation != -1)
	{
		glEnableVertexAttribArray(m_a_positionLocation);
		glVertexAttribPointer(m_a_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	}
	if (m_a_uvLocation != -1)
	{
		glEnableVertexAttribArray(m_a_uvLocation);
		glVertexAttribPointer(m_a_uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	}
	if (m_u_mainTextureLocation != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glUniform1i(m_u_mainTextureLocation, 0);
	}
}
