#pragma once
#include "Effect.h"
#include "MaterialParticleBox.h"
#include <vector>
#include "Particle.h"
class ParticlesEmitterBox : public Effect {
public:
	enum EmitType { PureRandom, AccumulativeCenter };
private:
	MaterialParticleBox* m_material;
	Texture* m_texture;
	GLuint m_particleBufferHandle;
	bool m_isBufferInitiated;

	int m_iNumOfParticles;
	float m_cycleTime;
	int m_iNumOfLoop;
	Vector2 m_emitRange;
	EmitType m_emitType;

	Vector2 m_velocityOffsetRandomRange;
	float m_velocityMulRandomRange;

	float m_timeOffsetRandomRange;
	float m_lifeTime;
	float m_lifeTimeRandomRange;

	float m_size;
	float m_sizeRandomRange;

	// color
	Vector4 m_initColor;
	Vector4 m_colorOffsetRandomRange;
	Vector4 m_endColor;
	float(*m_colorCurve)(float, float, float);

	Vector2 m_velocity;
	float m_timeCounter;
	int m_iLoopCounter;
public:
	ParticlesEmitterBox(Vector3 position, Vector2 scale, float rotation);
	ParticlesEmitterBox(ParticlesEmitterBox& ps);
	virtual ~ParticlesEmitterBox();
	void SetEmitInfo(Vector2 emitRange, EmitType emitType);
	void SetVelocityInfo(Vector2 velocityOffsetRandomRange, float velocityMulRandomRange);
	void SetTimeInfo(float timeOffsetRandomRange, float lifeTime, float lifeTimeRandomRange);
	void SetSizeInfo(float initSize, float randomRange);
	void SetColorInfo(Vector4 initColor, Vector4 randomRange, Vector4 endColor, float(*curve)(float, float, float));
	
	bool Init(MaterialParticleBox* material, Texture* texture, int iNumOfParticles, float cycleTime, int iNumOfLoop);
	void Play();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
	void SetVelocity(Vector2 newVelocity);
};