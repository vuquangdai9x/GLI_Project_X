#include "stdafx.h"
#include "Material.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Material::Material(int id)
{
	m_id = id;
}

bool Material::Init(int iShaderId)
{
	m_shader = ResourceManager::GetInstance()->GetShader(iShaderId);
	if (m_shader == NULL) {
		if (m_shader == NULL) printf("[ERR] Material: Failed to get shader %d\n", iShaderId);
		return false;
	}
	m_u_mainTextureLocation = glGetUniformLocation(m_shader->program, "u_mainTexture");

	m_a_positionLocation = glGetAttribLocation(m_shader->program, "a_posL");
	m_a_uvLocation = glGetAttribLocation(m_shader->program, "a_uv");
	m_u_wvpLocation = glGetUniformLocation(m_shader->program, "u_wvp");

	m_u_fogStartLocation = glGetUniformLocation(m_shader->program, "u_fogStart");
	m_u_fogLengthLocation = glGetUniformLocation(m_shader->program, "u_fogLength");
	m_u_fogColor = glGetUniformLocation(m_shader->program, "u_fogColor");
}

void Material::PrepareShader(Matrix& WVP)
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

	if (m_u_fogStartLocation != -1 && m_u_fogLengthLocation != -1 && m_u_fogColor != -1) {
		glUniform1f(m_u_fogStartLocation, SceneManager::GetInstance()->m_fogStart);
		glUniform1f(m_u_fogLengthLocation, SceneManager::GetInstance()->m_fogLength);
		glUniform4fv(m_u_fogColor, 1, (GLfloat*) & (SceneManager::GetInstance()->m_fogColor));
	}
}

void Material::SetMainTexture(Texture* mainTexture)
{
	m_mainTexure = mainTexture;
}

int Material::GetId()
{
	return m_id;
}
