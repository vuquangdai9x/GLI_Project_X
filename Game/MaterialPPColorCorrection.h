#pragma once
#include "MaterialPostProcess.h"
class MaterialPPColorCorrection: public MaterialPostProcess
{
protected:
	// location
	GLuint m_uLiftLocation;
	GLuint m_uGammaLocation;
	GLuint m_uGainLocation;
	GLuint m_uOffsetLocation;
	GLuint m_uContrastLocation;
	GLuint m_uContrastPivotLocation;
	GLuint m_uSaturationLocation;
	GLuint m_uHueLocation;
	GLuint m_uTemperatureLocation;
	GLuint m_uUnsharpMaskLocation;
	GLuint m_uResolutionLocation;
public:
	Vector3 m_uLift;
	Vector3 m_uGamma;
	Vector3 m_uGain;
	Vector3 m_uOffset;
	float m_uContrast;
	float m_uContrastPivot;
	float m_uSaturation;
	float m_uHue;
	float m_uTemperature;
	float m_uUnsharpMask;
	Vector2 m_uResolution;
public:
	MaterialPPColorCorrection(int id);
	~MaterialPPColorCorrection();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(GLuint textureHandle);
};

