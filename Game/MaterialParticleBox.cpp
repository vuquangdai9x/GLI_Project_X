#include "MaterialParticleBox.h"
#include "Particle.h"
#include "EffectManager.h"

MaterialParticleBox::MaterialParticleBox(int id)
{
	m_id = id;
	m_shader = NULL;
}

int MaterialParticleBox::GetId()
{
	return m_id;
}

bool MaterialParticleBox::Init(Shaders* shader)
{
	if (shader == NULL) {
		printf("[ERR] Material: shader is NULL\n");
		return false;
	}
	m_shader = shader;

	m_a_initPosLocation = glGetAttribLocation(m_shader->program, "a_initPos");
	m_a_velocityOffsetLocation = glGetAttribLocation(m_shader->program, "a_velocityOffset");
	m_a_velocityMulLocation = glGetAttribLocation(m_shader->program, "a_velocityMultiplier");
	m_a_timeOffsetLocation = glGetAttribLocation(m_shader->program, "a_timeOffset");
	m_a_lifeTimeLocation = glGetAttribLocation(m_shader->program, "a_lifeTime");
	m_a_sizeLocation = glGetAttribLocation(m_shader->program, "a_size");
	m_a_colorOffsetLocation = glGetAttribLocation(m_shader->program, "a_colorOffset");

	m_u_mainTextureLocation = glGetUniformLocation(m_shader->program, "u_mainTexture");
	m_u_wvpLocation = glGetUniformLocation(m_shader->program, "u_wvp");
	m_u_colorLocation = glGetUniformLocation(m_shader->program, "u_color");
	m_u_timeLocation = glGetUniformLocation(m_shader->program, "u_time");
	m_u_velocityLocation = glGetUniformLocation(m_shader->program, "u_velocity");

	return true;
}

void MaterialParticleBox::PrepareShader(Matrix & WVP, Texture * texture, float time, Vector2* velocity, Vector4 * color)
{
	glUseProgram(m_shader->program);
	if (m_a_initPosLocation != -1)
	{
		glEnableVertexAttribArray(m_a_initPosLocation);
		glVertexAttribPointer(m_a_initPosLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, initPos));
	}
	if (m_a_velocityOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_velocityOffsetLocation);
		glVertexAttribPointer(m_a_velocityOffsetLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, velocityOffset));
	}
	if (m_a_velocityMulLocation != -1)
	{
		glEnableVertexAttribArray(m_a_velocityMulLocation);
		glVertexAttribPointer(m_a_velocityMulLocation, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, velocityMul));
	}
	if (m_a_timeOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_timeOffsetLocation);
		glVertexAttribPointer(m_a_timeOffsetLocation, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, timeOffset));
	}
	if (m_a_lifeTimeLocation != -1)
	{
		glEnableVertexAttribArray(m_a_lifeTimeLocation);
		glVertexAttribPointer(m_a_lifeTimeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, lifeTime));
	}
	if (m_a_sizeLocation != -1)
	{
		glEnableVertexAttribArray(m_a_sizeLocation);
		glVertexAttribPointer(m_a_sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, size));
	}


	if (m_a_colorOffsetLocation != -1)
	{
		glEnableVertexAttribArray(m_a_colorOffsetLocation);
		glVertexAttribPointer(m_a_colorOffsetLocation, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleBox), (GLvoid*)offsetof(ParticleBox, colorOffset));
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
	if (m_u_timeLocation != -1) {
		glUniform1f(m_u_timeLocation, (GLfloat)time);
	}
	if (m_u_velocityLocation != -1) {
		glUniform2fv(m_u_velocityLocation, 1, (GLfloat*)velocity);
	}
}