#include "ParticlesEmitterBox.h"
#include "CurveFunction.h"

#include <random>

ParticlesEmitterBox::ParticlesEmitterBox(Vector3 position, Vector2 scale, float rotation) : Effect(position, scale, rotation)
{
	m_timeCounter = 0;
	m_isBufferInitiated = false;
}
ParticlesEmitterBox::ParticlesEmitterBox(ParticlesEmitterBox& ps) : Effect(ps)
{
	m_timeCounter = 0;
	SetEmitInfo(ps.m_emitRange, ps.m_emitType);
	SetVelocityInfo(ps.m_velocityOffsetRandomRange, ps.m_velocityMulRandomRange);
	SetVelocity(ps.m_velocity);
	SetTimeInfo(ps.m_timeOffsetRandomRange, ps.m_lifeTime, ps.m_lifeTimeRandomRange);
	SetSizeInfo(ps.m_size, ps.m_sizeRandomRange);
	SetColorInfo(ps.m_initColor, ps.m_colorOffsetRandomRange, ps.m_endColor, ps.m_colorCurve);
	m_isBufferInitiated = false;
	Init(ps.m_material, ps.m_texture, ps.m_iNumOfParticles, ps.m_cycleTime, ps.m_iNumOfLoop);
}
ParticlesEmitterBox::~ParticlesEmitterBox()
{
	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
}

void ParticlesEmitterBox::SetEmitInfo(Vector2 emitRange, EmitType emitType)
{
	m_emitRange = emitRange;
	m_emitType = emitType;
}

void ParticlesEmitterBox::SetVelocityInfo(Vector2 velocityOffsetRandomRange, float velocityMulRandomRange)
{
	m_velocityOffsetRandomRange = velocityOffsetRandomRange;
	m_velocityMulRandomRange = velocityMulRandomRange;
}

void ParticlesEmitterBox::SetTimeInfo(float timeOffsetRandomRange, float lifeTime, float lifeTimeRandomRange)
{
	m_timeOffsetRandomRange = timeOffsetRandomRange;
	m_lifeTime = lifeTime;
	m_lifeTimeRandomRange = lifeTimeRandomRange;
}

void ParticlesEmitterBox::SetSizeInfo(float initSize, float randomRange)
{
	m_size = initSize;
	m_sizeRandomRange = randomRange;
}

void ParticlesEmitterBox::SetColorInfo(Vector4 initColor, Vector4 randomRange, Vector4 endColor, float(*curve)(float, float, float)) {
	m_initColor = initColor;
	m_colorOffsetRandomRange = randomRange;
	m_endColor = endColor;
	m_colorCurve = curve;
}

bool ParticlesEmitterBox::Init(MaterialParticleBox* material, Texture* texture, int iNumOfParticles, float cycleTime, int iNumOfLoop)
{
	bool isSuccess = true;
	if (material == NULL) {
		printf("[ERR] ParticlesEmitterBox::AddEmitter: MaterialParticles is NULL\n");
		isSuccess = false;
	}
	if (texture == NULL) {
		printf("[ERR] ParticlesEmitterBox::AddEmitter: Texture is NULL\n");
		isSuccess = false;
	}
	if (iNumOfParticles <= 0) {
		printf("[ERR] ParticlesEmitterBox::AddEmitter: Num of particles less or equal zero\n");
		isSuccess = false;
	}
	if (cycleTime <= 0) {
		printf("[ERR] ParticlesEmitterBox::AddEmitter: Cycle time less or equal zero\n");
		isSuccess = false;
	}
	if (!isSuccess) return false;

	m_iNumOfParticles = iNumOfParticles;
	ParticleBox* aParticles = new ParticleBox[m_iNumOfParticles];

	m_cycleTime = cycleTime;
	m_iNumOfLoop = iNumOfLoop;
	m_texture = texture;
	m_material = material;

	// set up emit angle
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0, 1.0);
	switch (m_emitType)
	{
	case EmitType::AccumulativeCenter:
		for (int i = 0; i < m_iNumOfParticles; i++) {
			aParticles[i].initPos.x = distribution(generator) * m_emitRange.x;
			aParticles[i].initPos.y = distribution(generator) * m_emitRange.y;
		}
		break;
	case EmitType::PureRandom:
		for (int i = 0; i < m_iNumOfParticles; i++) {
			aParticles[i].initPos.x = ((float)rand() / (float)RAND_MAX * 2 - 1) * m_emitRange.x;
			aParticles[i].initPos.y = ((float)rand() / (float)RAND_MAX * 2 - 1) * m_emitRange.y;
		}
		break;
	}

	// set up other info
	float randomColor;
	for (int i = 0; i < m_iNumOfParticles; i++) {
		aParticles[i].velocityOffset.x = ((float)rand() / (float)RAND_MAX * 2 - 1) * m_velocityOffsetRandomRange.x;
		aParticles[i].velocityOffset.y = ((float)rand() / (float)RAND_MAX * 2 - 1) * m_velocityOffsetRandomRange.y;
		aParticles[i].velocityMul = 1 + ((float)rand() / (float)RAND_MAX * 2 - 1) * m_velocityMulRandomRange;
		aParticles[i].timeOffset = (float)rand() / (float)RAND_MAX * m_timeOffsetRandomRange;
		aParticles[i].lifeTime = m_lifeTime + ((float)rand() / (float)RAND_MAX * 2 - 1) * m_lifeTimeRandomRange;
		aParticles[i].size = (m_size + ((float)rand() / (float)RAND_MAX * 2 - 1) * m_sizeRandomRange) * texture->GetWidth();
		randomColor = (float)rand() / (float)RAND_MAX * 2 - 1;
		aParticles[i].colorOffset.x = randomColor * m_colorOffsetRandomRange.x;
		aParticles[i].colorOffset.y = randomColor * m_colorOffsetRandomRange.y;
		aParticles[i].colorOffset.z = randomColor * m_colorOffsetRandomRange.z;
		aParticles[i].colorOffset.w = ((float)rand() / (float)RAND_MAX * 2 - 1) * m_colorOffsetRandomRange.w;
	}

	// load to buffer
	if (m_isBufferInitiated) glDeleteBuffers(1, &m_particleBufferHandle);
	glGenBuffers(1, &(m_particleBufferHandle));
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleBox) * m_iNumOfParticles, aParticles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] aParticles;
	m_isBufferInitiated = true;

	return true;
}

void ParticlesEmitterBox::Play()
{
	m_timeCounter = 0;
	m_iLoopCounter = 0;
	SetActive(true);
}

void ParticlesEmitterBox::Update(float deltaTime)
{
	if (!m_isEnabled) return;
	m_timeCounter += deltaTime;
	if (m_timeCounter > m_cycleTime) {
		m_iLoopCounter++;
		if (m_iNumOfLoop > 0 && m_iLoopCounter >= m_iNumOfLoop) {
			SetActive(false);
		}
		else {
			m_timeCounter = 0;
		}
	}
}

void ParticlesEmitterBox::Render(Camera2D* mainCamera)
{
	if (!m_isEnabled) return;

	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();

	Vector4 color = m_initColor + (m_endColor - m_initColor) * m_colorCurve(0, 1, m_timeCounter / m_cycleTime);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferHandle);
	m_material->PrepareShader(
		m_WVP,
		m_texture,
		m_timeCounter,
		&m_velocity,
		&color
	);

	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_POINTS, 0, m_iNumOfParticles);

	glEnable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ParticlesEmitterBox::SetVelocity(Vector2 newVelocity)
{
	m_velocity = newVelocity;
}
