#include "MaterialPPColorCorrection.h"



MaterialPPColorCorrection::MaterialPPColorCorrection(int id): MaterialPostProcess(id)
{
	m_uLift = Vector3(0,0,0);
	m_uGamma = Vector3(0, 0, 0);
	m_uGain = Vector3(0, 0, 0);
	m_uOffset = Vector3(0, 0, 0);
	m_uContrast = 1.0;
	m_uContrastPivot = 0.5;
	m_uSaturation = 1.0;
	m_uHue = 0.0;
	m_uTemperature = 66.0;
	m_uUnsharpMask = 0.0;
	m_uResolution = Vector2(Globals::screenWidth, Globals::screenHeight);
}


MaterialPPColorCorrection::~MaterialPPColorCorrection()
{
}

bool MaterialPPColorCorrection::Init(Shaders * shader)
{
	bool isSuccess = MaterialPostProcess::Init(shader);
	if (!isSuccess) return false;
	m_uLiftLocation = glGetUniformLocation(m_shader->program, "uLift");
	m_uGammaLocation = glGetUniformLocation(m_shader->program, "uGamma");
	m_uGainLocation = glGetUniformLocation(m_shader->program, "uGain");
	m_uOffsetLocation = glGetUniformLocation(m_shader->program, "uOffset");
	m_uContrastLocation = glGetUniformLocation(m_shader->program, "uContrast");
	m_uContrastPivotLocation = glGetUniformLocation(m_shader->program, "uContrastPivot");
	m_uSaturationLocation = glGetUniformLocation(m_shader->program, "uSaturation");
	m_uHueLocation = glGetUniformLocation(m_shader->program, "uHue");
	m_uTemperatureLocation = glGetUniformLocation(m_shader->program, "uTemperature");
	m_uUnsharpMaskLocation = glGetUniformLocation(m_shader->program, "uUnsharpMask");
	m_uResolutionLocation = glGetUniformLocation(m_shader->program, "uResolution");
	return true;
}

void MaterialPPColorCorrection::PrepareShader(GLuint textureHandle)
{
	MaterialPostProcess::PrepareShader(textureHandle);
	if (m_uLiftLocation != -1) {
		glUniform3fv(m_uLiftLocation, 1, (GLfloat*)&m_uLift);
	}
	if (m_uGammaLocation != -1) {
		glUniform3fv(m_uGammaLocation, 1, (GLfloat*)&m_uGamma);
	}
	if (m_uGainLocation != -1) {
		glUniform3fv(m_uGainLocation, 1, (GLfloat*)&m_uGain);
	}
	if (m_uOffsetLocation != -1) {
		glUniform3fv(m_uOffsetLocation, 1, (GLfloat*)&m_uOffset);
	}
	if (m_uContrastLocation != -1) {
		glUniform1f(m_uContrastLocation, (GLfloat)m_uContrast);
	}
	if (m_uContrastPivotLocation != -1) {
		glUniform1f(m_uContrastPivotLocation, (GLfloat)m_uContrastPivot);
	}
	if (m_uSaturationLocation != -1) {
		glUniform1f(m_uSaturationLocation, (GLfloat)m_uSaturation);
	}
	if (m_uHueLocation != -1) {
		glUniform1f(m_uHueLocation, (GLfloat)m_uHue);
	}
	if (m_uTemperatureLocation != -1) {
		glUniform1f(m_uTemperatureLocation, (GLfloat)m_uTemperature);
	}
	if (m_uUnsharpMaskLocation != -1) {
		glUniform1f(m_uUnsharpMaskLocation, (GLfloat)m_uUnsharpMask);
	}
	if (m_uResolutionLocation != -1) {
		glUniform2fv(m_uResolutionLocation, 1, (GLfloat*)&m_uResolution);
	}
}
