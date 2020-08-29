#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class MaterialParticle2D{
protected:
	int m_id;
	Shaders* m_shader;
	//Texture* m_mainTexure;
	GLint m_u_mainTextureLocation = -1;
	//GLint m_a_uvLocation = -1;
	GLint m_u_wvpLocation = -1;

	//GLint m_u_sizeLocation = -1;
	//GLint m_u_offsetLocation = -1;

	GLint m_u_colorLocation = -1;
	GLint m_u_pointSizeLocation = -1;

	GLint m_u_KLocation = -1;
	GLint m_a_thetaLocation = -1;
public:
	MaterialParticle2D(int id);
	int GetId();
	virtual bool Init(Shaders* shader);
	virtual void PrepareShader(Matrix& WVP, float k, float pointSize, Texture* texture, Vector4* color);
};