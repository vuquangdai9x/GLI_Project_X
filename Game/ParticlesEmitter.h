#pragma once
#include "Effect.h"
#include "MaterialParticle2D.h"
#include <vector>
#include "Particle.h"
class ParticlesEmitter : public Effect {
private:
	MaterialParticle2D* m_material;
	Texture* m_texture;
	GLuint m_particleBufferHandle;
	bool m_isBufferInitiated;
	int m_iNumOfParticles;
	float m_cycleTime;
	int m_iNumOfLoop;

	float m_k;
	float m_size;
	Vector4 m_color;

	float m_timeCounter;
public:
	/*ParticlesEmitter(int id);
	ParticlesEmitter(int id, ParticlesEmitter& ps);*/
	ParticlesEmitter(Vector3 position, Vector2 scale, float rotation);
	ParticlesEmitter(ParticlesEmitter& ps);
	virtual ~ParticlesEmitter();
	bool Init(MaterialParticle2D* material, Texture* texture, int iNumOfParticles, float k, float size, Vector4 color, float cycleTime, int iNumOfLoop);
	void Play();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
};