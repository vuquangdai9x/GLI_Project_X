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
	m_u_sizeLocation = glGetUniformLocation(m_shader->program, "u_size");
	m_u_radiusLocation = glGetUniformLocation(m_shader->program, "u_radius");

	m_a_angleLocation = glGetAttribLocation(m_shader->program, "a_angle");
	m_a_radiusOffsetLocation = glGetAttribLocation(m_shader->program, "a_radiusOffset");
	m_a_radiusMulLocation = glGetAttribLocation(m_shader->program, "a_radiusMul");
	m_a_sizeOffsetLocation = glGetAttribLocation(m_shader->program, "a_sizeOffset");
	m_a_colorOffsetLocation = glGetAttribLocation(m_shader->program, "a_colorOffset");

	return true;
}

void MaterialParticle2D::PrepareShader(Matrix& WVP, Texture* texture, float radius, float size, Vector4* color)
{
	glUseProgram(m_shader->program);
	if (m_a_angleLocation != -1)
	{
		glEnableVertexAttribArray(m_a_angleLocation);
		glVertexAttribPointer(m_a_angleLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, angle));
	}
	if (m_a_radiusOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_radiusOffsetLocation);
		glVertexAttribPointer(m_a_radiusOffsetLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, radiusOffset));
	}
	if (m_a_radiusMulLocation != -1)
	{
		glEnableVertexAttribArray(m_a_radiusMulLocation);
		glVertexAttribPointer(m_a_radiusMulLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, radiusMultiplier));
	}
	if (m_a_sizeOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_sizeOffsetLocation);
		glVertexAttribPointer(m_a_sizeOffsetLocation, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, sizeOffset));
	}
	if (m_a_colorOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_colorOffsetLocation);
		glVertexAttribPointer(m_a_colorOffsetLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, colorOffset));
	}

	if (m_u_wvpLocation != -1) {
		glUniformMatrix4fv(m_u_wvpLocation, 1, GL_FALSE, (GLfloat*)& WVP);
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

	if (m_u_colorLocation != -1) {
		glUniform4fv(m_u_colorLocation, 1, (GLfloat*)color);
	}
	if (m_u_sizeLocation != -1) {
		glUniform1f(m_u_sizeLocation, (GLfloat)size);
	}
	if (m_u_radiusLocation != -1) {
		glUniform1f(m_u_radiusLocation, (GLfloat)radius);
	}
}