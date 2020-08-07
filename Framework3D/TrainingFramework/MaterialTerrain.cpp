#include "stdafx.h"
#include "MaterialTerrain.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Globals.h"

MaterialTerrain::MaterialTerrain(int id) : Material(id)
{
}

bool MaterialTerrain::Init(int iShaderId, int iBlendTextureId, int iTexture1Id, int iTexture2Id, int iTexture3Id)
{
	bool isLoadSuccessfully = true;
	isLoadSuccessfully = Material::Init(iShaderId);
	if (!isLoadSuccessfully) return false;

	m_u_blendtexLocation = glGetUniformLocation(m_shader->program, "u_texture[0]");
	m_u_Tex1Location = glGetUniformLocation(m_shader->program, "u_texture[1]");
	m_u_Tex2Location = glGetUniformLocation(m_shader->program, "u_texture[2]");
	m_u_Tex3Location = glGetUniformLocation(m_shader->program, "u_texture[3]");

	m_blendTexture = ResourceManager::GetInstance()->GetTexture(iBlendTextureId);
	if (m_blendTexture == NULL) {
		printf("[ERR] Material Single Texture: Failed to get blend texture %d\n", iBlendTextureId);
		isLoadSuccessfully = false;
	}
	m_texture1 = ResourceManager::GetInstance()->GetTexture(iTexture1Id);
	if (m_texture1 == NULL) {
		printf("[ERR] Material Single Texture: Failed to get texture1 %d\n", iTexture1Id);
		isLoadSuccessfully = false;
	}
	m_texture2 = ResourceManager::GetInstance()->GetTexture(iTexture2Id);
	if (m_texture2 == NULL) {
		printf("[ERR] Material Single Texture: Failed to get texture2 %d\n", iTexture2Id);
		isLoadSuccessfully = false;
	}
	m_texture3 = ResourceManager::GetInstance()->GetTexture(iTexture3Id);
	if (m_texture3 == NULL) {
		printf("[ERR] Material Single Texture: Failed to get texture3 %d\n", iTexture3Id);
		isLoadSuccessfully = false;
	}
	return isLoadSuccessfully;
}

void MaterialTerrain::PrepareShader(Matrix & WVP)
{
	Material::PrepareShader(WVP);

	glUseProgram(m_shader->program);
	if (m_u_blendtexLocation != -1 && m_blendTexture != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_blendTexture->GetTextureHandle());
		glUniform1i(m_u_blendtexLocation, 0);
	}
	if (m_u_Tex1Location != -1 && m_texture1 != NULL) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture1->GetTextureHandle());
		glUniform1i(m_u_Tex1Location, 1);
	}
	if (m_u_Tex2Location != -1 && m_texture2 != NULL) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_texture2->GetTextureHandle());
		glUniform1i(m_u_Tex2Location, 2);
	}
	if (m_u_Tex3Location != -1 && m_texture3 != NULL) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_texture3->GetTextureHandle());
		glUniform1i(m_u_Tex3Location, 3);
	}
}
