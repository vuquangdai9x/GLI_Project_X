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
	SetAngleInfo(ps.m_emitAngle, ps.m_emitAngleRandomRange, ps.m_emitType);
	SetRadiusInfo(ps.m_initRadius, ps.m_radiusOffsetRandomRange, ps.m_endRadius, ps.m_radiusCurve);
	//SetVelocityInfo(ps.m_initVelocity, ps.m_velocityOffsetRandomRange, ps.m_endVelocity, ps.m_velocityCurve);
	SetSizeInfo(ps.m_initSize, ps.m_sizeOffsetRandomRange, ps.m_endSize, ps.m_sizeCurve);
	SetColorInfo(ps.m_initColor, ps.m_colorOffsetRandomRange, ps.m_endColor, ps.m_colorCurve);
	m_isBufferInitiated = false;
	Init(ps.m_material, ps.m_texture, ps.m_iNumOfParticles, ps.m_cycleTime, ps.m_iNumOfLoop);
}
ParticlesEmitter::~ParticlesEmitter()
{
	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
}

void ParticlesEmitter::SetAngleInfo(float emitAngle, float randomRange, EmitType emitType) {
	m_emitAngle = emitAngle;
	m_emitAngleRandomRange = randomRange;
	m_emitType = emitType;
}
void ParticlesEmitter::SetRadiusInfo(float initRadius, float randomRange, float endRadius, float (*curve)(float, float, float)) {
	m_initRadius = initRadius;
	m_radiusOffsetRandomRange = randomRange;
	m_endRadius = endRadius;
	m_radiusCurve = curve;
}
//void ParticlesEmitter::SetVelocityInfo(float initVelocity, float randomRange, float endVelocity, float (*curve)(float, float, float)) {
//	m_initVelocity = initVelocity;
//	m_velocityOffsetRandomRange = randomRange;
//	m_endVelocity = endVelocity;
//	m_velocityCurve = curve;
//}
void ParticlesEmitter::SetSizeInfo(float initSize, float randomRange, float endSize, float (*curve)(float, float, float)) {
	m_initSize = initSize;
	m_sizeOffsetRandomRange = randomRange;
	m_endSize = endSize;
	m_sizeCurve = curve;
}
void ParticlesEmitter::SetColorInfo(Vector4 initColor, Vector4 randomRange, Vector4 endColor, float (*curve)(float, float, float)) {
	m_initColor = initColor;
	m_colorOffsetRandomRange = randomRange;
	m_endColor = endColor;
	m_colorCurve = curve;
}

bool ParticlesEmitter::Init(MaterialParticle2D* material, Texture* texture, int iNumOfParticles, float cycleTime, int iNumOfLoop)
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
	if (!isSuccess) return false;
	
	m_iNumOfParticles = iNumOfParticles;
	Particle* aParticles = new Particle[m_iNumOfParticles];

	m_cycleTime = cycleTime;
	m_iNumOfLoop = iNumOfLoop;
	m_texture = texture;
	m_material = material;

	// set up emit angle
	switch (m_emitType)
	{
	case EmitType::PureRandom:
		for (int i = 0;i < m_iNumOfParticles; i++) {
			aParticles[i].angle = m_emitAngle + ((float)rand() / (float)RAND_MAX * 2 - 1) * m_emitAngleRandomRange;
		}
		break;
	case EmitType::Distributed:
	default:
		for (int i = 0;i < m_iNumOfParticles; i++) {
			aParticles[i].angle = m_emitAngle + (((float)rand() / (float)RAND_MAX + i) / m_iNumOfParticles - 0.5) * m_emitAngleRandomRange;
		}
		break;
	}
	
	// set up other info
	for (int i = 0;i < m_iNumOfParticles; i++) {
		aParticles[i].radiusOffset = (float)rand() / (float)RAND_MAX * m_radiusOffsetRandomRange;
		//aParticles[i].velocityOffset = (float)rand() / (float)RAND_MAX * m_velocityOffsetRandomRange;
		aParticles[i].sizeOffset = (float)rand() / (float)RAND_MAX * m_sizeOffsetRandomRange;
		aParticles[i].colorOffset.x = (float)rand() / (float)RAND_MAX * m_colorOffsetRandomRange.x;
		aParticles[i].colorOffset.y = (float)rand() / (float)RAND_MAX * m_colorOffsetRandomRange.y;
		aParticles[i].colorOffset.z = (float)rand() / (float)RAND_MAX * m_colorOffsetRandomRange.z;
		aParticles[i].colorOffset.w = (float)rand() / (float)RAND_MAX * m_colorOffsetRandomRange.w;
	}

	// load to buffer
	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
	glGenBuffers(1, &(m_particleBufferHandle));
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_iNumOfParticles, aParticles, GL_STATIC_DRAW);
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

	Vector4 color = m_initColor + (m_endColor - m_initColor) * m_colorCurve(0, 1, fractpart);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	m_material->PrepareShader(
		m_WVP, 
		m_texture, 
		m_radiusCurve(m_initRadius, m_endRadius, fractpart), 
		m_sizeCurve(m_initSize, m_endSize, fractpart),
		&color
	);

	glDrawArrays(GL_POINTS, 0, m_iNumOfParticles);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
