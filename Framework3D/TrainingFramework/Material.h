#pragma once
#include "../Utilities/utilities.h"
#include "Shaders.h"
#include "Texture.h"

class Material {
protected:
	int m_id;
	Shaders* m_shader;
	Texture* m_mainTexure;

	GLint m_u_mainTextureLocation = -1;
	GLint m_a_positionLocation = -1;
	GLint m_a_uvLocation = -1;
	GLint m_a_colorLocation = -1;
	GLint m_u_wvpLocation = -1;
	GLint m_u_fogStartLocation = -1, m_u_fogLengthLocation = -1, m_u_fogColor = -1;
public:
	Material(int id);
	int GetId();
	virtual bool Init(int iShaderId);
	virtual void PrepareShader(Matrix & WVP);
	virtual void SetMainTexture(Texture* mainTexture);
};