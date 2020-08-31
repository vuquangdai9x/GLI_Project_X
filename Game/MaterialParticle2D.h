#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class MaterialParticle2D{
protected:
	int m_id;
	Shaders* m_shader;
	GLint m_u_mainTextureLocation;
	GLint m_u_wvpLocation;

	GLint m_a_angleLocation;
	GLint m_a_radiusOffsetLocation;
	GLint m_a_colorOffsetLocation;
	GLint m_a_sizeOffsetLocation;
	//GLint m_a_velocityOffsetLocation;

	GLint m_u_radiusLocation;
	GLint m_u_colorLocation;
	GLint m_u_sizeLocation;
	//GLint m_u_velocityLocation;
public:
	MaterialParticle2D(int id);
	int GetId();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(Matrix& WVP, Texture* texture, float radius, float size,Vector4* color);
};