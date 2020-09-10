#pragma once
#include "Effect.h"
#include "MaterialParticle2D.h"
#include <vector>
#include "Particle.h"
class ParticlesEmitter : public Effect {
public:
	enum EmitType { PureRandom, Distributed };
private:
	MaterialParticle2D* m_material;
	Texture* m_texture;
	GLuint m_particleBufferHandle;
	bool m_isBufferInitiated;

	int m_iNumOfParticles;
	float m_cycleTime;
	int m_iNumOfLoop;

	// emit angle
	float m_emitAngle;
	float m_emitAngleRandomRange;
	EmitType m_emitType;

	// init radius
	float m_initRadius;
	float m_radiusOffsetRandomRange;
	float m_radiusMulRandomRange;
	float m_endRadius;
	float (*m_radiusCurve)(float, float, float);

	// velocity
	/*float m_initVelocity;
	float m_velocityOffsetRandomRange;
	float m_endVelocity;
	float (*m_velocityCurve)(float, float, float);*/

	// size
	float m_initSize;
	float m_sizeOffsetRandomRange;
	float m_endSize;
	float (*m_sizeCurve)(float, float, float);

	// color
	Vector4 m_initColor;
	Vector4 m_colorOffsetRandomRange;
	Vector4 m_endColor;
	float (*m_colorCurve)(float, float, float);

	float m_timeCounter;
public:
	/*ParticlesEmitter(int id);
	ParticlesEmitter(int id, ParticlesEmitter& ps);*/
	ParticlesEmitter(Vector3 position, Vector2 scale, float rotation);
	ParticlesEmitter(ParticlesEmitter& ps);
	virtual ~ParticlesEmitter();
	void SetAngleInfo(float emitAngle, float randomRange, EmitType emitType);
	void SetRadiusInfo(float initRadius, float offsetRandomRange, float mulRandomRange, float endRadius, float (*curve)(float, float, float));
	//void SetVelocityInfo(float initVelocity, float randomRange, float endVelocity, float (*curve)(float, float, float));
	void SetSizeInfo(float initSize, float randomRange, float endSize, float (*curve)(float, float, float));
	void SetColorInfo(Vector4 initColor, Vector4 randomRange, Vector4 endColor, float (*curve)(float, float, float));
	bool Init(MaterialParticle2D* material, Texture* texture, int iNumOfParticles, float cycleTime, int iNumOfLoop);
	void Play();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
};
