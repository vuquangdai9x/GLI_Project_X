#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class MaterialText2D : public Material2D {
protected:
	GLuint m_uiTextureHandle;
	bool m_isSetTextureHandle;
public:
	MaterialText2D(int id);
	virtual bool Init(int iShaderId);
	virtual void PrepareShader(Matrix& WVP, float textureWidth, float textureHeight, float offsetX, float offsetY, float subWidth, float subHeight, Vector4* color);
	virtual void SetTextureHandle(GLuint handle);
};