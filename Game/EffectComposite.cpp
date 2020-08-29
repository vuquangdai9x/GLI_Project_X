#include "EffectComposite.h"
#include "ParticlesEmitter.h"

EffectComposite::EffectComposite(int id): m_id(id)
{
	m_position = Vector3(0, 0, 0);
	m_isActive = false;
}

EffectComposite::EffectComposite(int id, EffectComposite& ec) :m_id(id) {
	for (int i = 0;i < ec.m_listEffect.size();i++) {
		ParticlesEmitter* templateEmitter;
		if (templateEmitter = dynamic_cast<ParticlesEmitter*>(ec.m_listEffect[i])) {
			AddEffect(new ParticlesEmitter(*templateEmitter));
		}
		else {

		}
	}
	m_position = ec.m_position;
	m_isActive = ec.m_isActive;
}

EffectComposite::~EffectComposite()
{
	for (int i = 0;i < m_listEffect.size();i++) {
		delete m_listEffect[i];
	}
}

int EffectComposite::GetId() { return m_id; }

bool EffectComposite::AddEffect(Effect* effect)
{
	if (effect == NULL) return false;
	m_listEffect.push_back(effect);
	return true;
}

void EffectComposite::Play(Vector3 position)
{
	Vector3 deltaPos = position - m_position;
	m_position = position;
	for (int i = 0;i < m_listEffect.size();i++) {
		m_listEffect[i]->SetPosition(m_listEffect[i]->GetPosition() + deltaPos);
		//m_listEffect[i]->SetPosition(position);
		m_listEffect[i]->Play();
	}
	SetActive(true);
}

void EffectComposite::Update(float deltaTime)
{
	if (!m_isActive) return;
	bool isExistEffectRunning = false;
	for (int i = 0;i < m_listEffect.size();i++) {
		if (m_listEffect[i]->CheckIsActive()) {
			m_listEffect[i]->Update(deltaTime);
			isExistEffectRunning = true;
		}
	}
	if (!isExistEffectRunning) {
		SetActive(false);
	}
}

void EffectComposite::Render(Camera2D* camera)
{
	if (!m_isActive) return;
	for (int i = 0;i < m_listEffect.size();i++) {
		m_listEffect[i]->Render(camera);
	}
}

void EffectComposite::SetActive(bool value) {
	m_isActive = value;
}
bool EffectComposite::CheckIsActive() { return m_isActive; }
