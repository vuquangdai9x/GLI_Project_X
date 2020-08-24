#include "MaterialText2D.h"
MaterialText2D::MaterialText2D(int id) :Material2D(id), m_isSetTextureHandle(false)
{}
bool MaterialText2D::Init(int iShaderId) {
	return Material2D::Init(iShaderId);
}
void MaterialText2D::SetTextureHandle(GLuint handle) {
	m_uiTextureHandle = handle;
	m_isSetTextureHandle = true;
}
void MaterialText2D::PrepareShader(Matrix& WVP, float textureWidth, float textureHeight, float offsetX, float offsetY, float subWidth, float subHeight, Vector4* color) {
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
	if (m_u_mainTextureLocation != -1 && m_isSetTextureHandle) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureHandle);
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