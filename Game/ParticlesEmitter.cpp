#include "ParticlesEmitter.h"
#include "CurveFunction.h"

#include <stdlib.h> 
#include <time.h>

//ParticlesEmitter::ParticlesEmitter(int id): Effect(id)
//{
//	m_timeCounter = 0;
//}
//ParticlesEmitter::ParticlesEmitter(int id, ParticlesEmitter& ps) : Effect(id, ps) 
//{
//	
//}
ParticlesEmitter::ParticlesEmitter(Vector3 position, Vector2 scale, float rotation): Effect(position,scale,rotation)
{
	m_timeCounter = 0;
	m_isBufferInitiated = false;
}
ParticlesEmitter::ParticlesEmitter(ParticlesEmitter& ps) : Effect(ps)
{
	m_timeCounter = 0;

	m_iNumOfParticles = ps.m_iNumOfParticles;
	Particle* aParticles = new Particle[m_iNumOfParticles];

	srand(time(NULL));
	for (int i = 0;i < m_iNumOfParticles;i++) {
		aParticles[i].theta = i / (float)m_iNumOfParticles * 2 * M_PI;
	}

	m_k = ps.m_k;
	m_cycleTime = ps.m_cycleTime;
	m_iNumOfLoop = ps.m_iNumOfLoop;
	m_size = ps.m_size;
	m_color = ps.m_color;

	m_material = ps.m_material;
	m_texture = ps.m_texture;

	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
	glGenBuffers(1, &(m_particleBufferHandle));
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_iNumOfParticles, aParticles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] aParticles;

	m_isBufferInitiated = true;
}
ParticlesEmitter::~ParticlesEmitter()
{
	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
}

bool ParticlesEmitter::Init(MaterialParticle2D* material, Texture* texture, int iNumOfParticles, float k, float size, Vector4 color, float cycleTime, int iNumOfLoop)
{
	bool isSuccess = true;
	if (material == NULL) {
		printf("[ERR] ParticlesEmitter::AddEmitter: MaterialParticles is NULL\n");
		isSuccess = false;
	}
	if (texture == NULL) {
		printf("[ERR] ParticlesEmitter::AddEmitter: Texture is NULL\n");
		isSuccess = false;
	}
	if (iNumOfParticles <= 0) {
		printf("[ERR] ParticlesEmitter::AddEmitter: Num of particles less or equal zero\n");
		isSuccess = false;
	}
	if (cycleTime <= 0) {
		printf("[ERR] ParticlesEmitter::AddEmitter: Cycle time less or equal zero\n");
		isSuccess = false;
	}
	if (size <= 0) {
		printf("[ERR] ParticlesEmitter::AddEmitter: Size less or equal zero\n");
		isSuccess = false;
	}
	if (!isSuccess) return false;

	
	m_iNumOfParticles = iNumOfParticles;
	Particle* aParticles = new Particle[m_iNumOfParticles];
	for (int i = 0;i < m_iNumOfParticles; i++) {
		aParticles[i].theta = i / (float)m_iNumOfParticles * 2 * M_PI;
	}

	m_k = k;
	m_cycleTime = cycleTime;
	m_iNumOfLoop = iNumOfLoop;
	m_size = size;
	m_color = color;
	m_texture = texture;
	m_material = material;

	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
	glGenBuffers(1, &(m_particleBufferHandle));
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_iNumOfParticles,	aParticles,	GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] aParticles;

	m_isBufferInitiated = true;
	return true;
}

void ParticlesEmitter::Play()
{
	m_timeCounter = 0;
	SetActive(true);
}

void ParticlesEmitter::Update(float deltaTime)
{
	if (!m_isEnabled) return;
	m_timeCounter += deltaTime;
	if (m_timeCounter / m_cycleTime >= m_iNumOfLoop) {
		SetActive(false);
	}
		
}

void ParticlesEmitter::Render(Camera2D* mainCamera)
{
	if (!m_isEnabled) return;
	// check if emitter is loop enough
	double fractpart, intpart;
	fractpart = modf(m_timeCounter / m_cycleTime, &intpart);

	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();

	float k, pointSize;
	Vector4 color(1.0, 0.0, 0.0, 1.0);
	k = m_k * fractpart;
	pointSize = m_size * fractpart;

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	m_material->PrepareShader(m_WVP, k, pointSize, m_texture, &color);

	glDrawArrays(GL_POINTS, 0, m_iNumOfParticles);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
