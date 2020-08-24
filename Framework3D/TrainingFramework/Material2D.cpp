#include "stdafx.h"
#include "Material2D.h"
#include "ResourcesManager2D.h"
#include "Singleton.h"

Material2D::Material2D(int id)
{
	m_id = id;
	m_mainTexure = NULL;
	m_shader = NULL;
}

void Material2D::SetMainTexture(Texture* mainTexture)
{
	m_mainTexure = mainTexture;
}

int Material2D::GetId()
{
	return m_id;
}

bool Material2D::Init(int iShaderId)
{
	m_shader = Singleton<ResourceManager2D>::GetInstance()->GetShader(iShaderId);
	if (m_shader == NULL) {
		printf("[ERR] Material: Failed to get shader %d\n", iShaderId);
		return false;
	}
	m_u_mainTextureLocation = glGetUniformLocation(m_shader->program, "u_mainTexture");
	m_a_positionLocation = glGetAttribLocation(m_shader->program, "a_posL");
	m_a_uvLocation = glGetAttribLocation(m_shader->program, "a_uv");
	m_u_wvpLocation = glGetUniformLocation(m_shader->program, "u_wvp");
	m_u_sizeLocation = glGetUniformLocation(m_shader->program, "u_size");
	m_u_offsetLocation = glGetUniformLocation(m_shader->program, "u_offset");

	m_u_colorLocation = glGetUniformLocation(m_shader->program, "u_color");
	m_u_originSizeLocation = glGetUniformLocation(m_shader->program, "u_originSize");
	return true;
}

void Material2D::PrepareShader(Matrix& WVP, float textureWidth, float textureHeight, float offsetX, float offsetY, float subWidth, float subHeight, Vector4* color)
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
	if (m_u_wvpLocation != -1) {
		glUniformMatrix4fv(m_u_wvpLocation, 1, GL_FALSE, (GLfloat*)& WVP);
	}
	if (m_u_mainTextureLocation != -1 && m_mainTexure != NULL) {
		if (m_mainTexure->IsCubeTexture()) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_mainTexure->GetTextureHandle());
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mainTexure->GetTextureHandle());
		}
		glUniform1i(m_u_mainTextureLocation, 0);
	}
	if (m_u_sizeLocation != -1) {
		glUniform2f(m_u_sizeLocation, (GLfloat)subWidth, (GLfloat)subHeight);
	}
	if (m_u_offsetLocation != -1) {
		glUniform2f(m_u_offsetLocation, (GLfloat)offsetX, (GLfloat)offsetY);
	}
	if (m_u_colorLocation != -1) {
		glUniform4fv(m_u_colorLocation, 1, (GLfloat*)color);
	}
	if (m_u_originSizeLocation != -1) {
		glUniform2f(m_u_originSizeLocation, (GLfloat)textureWidth, (GLfloat)textureHeight);
	}
}