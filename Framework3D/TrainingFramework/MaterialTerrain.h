#pragma once
#include "Material.h"
#include "Texture.h"
class MaterialTerrain : public Material {
private:
	Texture *m_blendTexture, *m_texture1, *m_texture2, *m_texture3;

	GLint m_u_blendtexLocation;
	GLint m_u_Tex1Location;
	GLint m_u_Tex2Location;
	GLint m_u_Tex3Location;

public:
	MaterialTerrain(int id);
	bool Init(int iShaderId, int iBlendTextureId, int iTexture1Id, int iTexture2Id, int iTexture3Id);
	void PrepareShader(Matrix & WVP);
};