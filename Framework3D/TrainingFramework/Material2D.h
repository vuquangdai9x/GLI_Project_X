#pragma once
#include "stdafx.h"
#include "Shaders.h"
#include "Texture.h"
class Material2D{
protected:
	int m_id;
	Shaders* m_shader;
	Texture* m_mainTexure;

	GLint m_u_mainTextureLocation = -1;
	GLint m_a_positionLocation = -1;
	GLint m_a_uvLocation = -1;
	GLint m_u_wvpLocation = -1;
	
	GLint m_u_sizeLocation = -1;
	GLint m_u_offsetLocation = -1;

	GLint m_u_colorLocation = -1;
public:
	Material2D(int id);
	int GetId();
	virtual void SetMainTexture(Texture* mainTexture);
	bool Init(int iShaderId);
	void PrepareShader(Matrix& WVP, float offsetX, float offsetY, float subWidth, float subHeight, Vector4* color);
};