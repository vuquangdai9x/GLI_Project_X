#include "MaterialParticle2D.h"
#include "Particle.h"
#include "EffectManager.h"

MaterialParticle2D::MaterialParticle2D(int id)
{
	m_id = id;
	m_shader = NULL;
}

int MaterialParticle2D::GetId()
{
	return m_id;
}

bool MaterialParticle2D::Init(Shaders* shader)
{
	if (shader == NULL) {
		printf("[ERR] Material: shader is NULL\n");
		return false;
	}
	m_shader = shader;
	m_u_mainTextureLocation = glGetUniformLocation(m_shader->program, "u_mainTexture");
	m_u_wvpLocation = glGetUniformLocation(m_shader->program, "u_wvp");
	m_u_colorLocation = glGetUniformLocation(m_shader->program, "u_color");
	m_u_pointSizeLocation = glGetUniformLocation(m_shader->program, "u_pointSize");
	m_u_KLocation = glGetUniformLocation(m_shader->program, "u_k");
	m_a_thetaLocation = glGetAttribLocation(m_shader->program, "a_theta");
	return true;
}

void MaterialParticle2D::PrepareShader(Matrix& WVP, float k, float pointSize, Texture* texture, Vector4* color)
{
	glUseProgram(m_shader->program);
	if (m_a_thetaLocation != -1)
	{
		glEnableVertexAttribArray(m_a_thetaLocation);
		glVertexAttribPointer(m_a_thetaLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, theta));
	}
	if (m_u_wvpLocation != -1) {
		glUniformMatrix4fv(m_u_wvpLocation, 1, GL_FALSE, (GLfloat*)& WVP);
	}
	if (m_u_colorLocation != -1) {
		glUniform4fv(m_u_colorLocation, 1, (GLfloat*)color);
	}
	if (m_u_mainTextureLocation != -1 && texture != NULL) {
		if (texture->IsCubeTexture()) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->GetTextureHandle());
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->GetTextureHandle());
		}
		glUniform1i(m_u_mainTextureLocation, 0);
	}
	if (m_u_pointSizeLocation != -1) {
		glUniform1f(m_u_pointSizeLocation, (GLfloat)pointSize);
	}
	if (m_u_KLocation != -1) {
		glUniform1f(m_u_KLocation, (GLfloat)k);
	}
}