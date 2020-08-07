#pragma once
#include "Material.h"
#include "Texture.h"
class MaterialFire : public Material {
private:
	Texture * m_displTexture, * m_alphaMaskTexture;
	float m_displMax, m_timeScale;

	GLint m_u_displTexLocation = -1;
	GLint m_u_alphaMaskTexLocation = -1;
	GLint m_u_timeLocation = -1;
	GLint m_u_displMaxLocation = -1;
public:
	MaterialFire(int id);
	bool Init(int iShaderId, int iDisplTextureId, int iAlphaMaskTexture2Id, float displMax, float timeScale);
	void PrepareShader(Matrix& WVP);
};