#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class MaterialParticleBox {
protected:
	int m_id;
	Shaders* m_shader;
	
	GLint m_a_initPosLocation;
	GLint m_a_velocityOffsetLocation;
	GLint m_a_velocityMulLocation;
	GLint m_a_timeOffsetLocation;
	GLint m_a_lifeTimeLocation;
	GLint m_a_sizeLocation;
	GLint m_a_colorOffsetLocation;
	
	GLint m_u_mainTextureLocation;
	GLint m_u_wvpLocation;
	GLint m_u_timeLocation;
	GLint m_u_velocityLocation;
	GLint m_u_colorLocation;

public:
	MaterialParticleBox(int id);
	int GetId();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(Matrix& WVP, Texture* texture, float time, Vector2* velocity, Vector4* color);
};