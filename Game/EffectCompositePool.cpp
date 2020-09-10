#include "EffectCompositePool.h"

int EffectCompositePool::GetId() { return m_id; }
EffectCompositePool::EffectCompositePool(int id, int iInitAmount, EffectComposite* templateEffComposite) 
	:m_id(id) 
{
	m_templateEffComposite = templateEffComposite;
	m_currentEffCompId = templateEffComposite->GetId();
	for (int i = 0;i < iInitAmount;i++) {
		m_currentEffCompId++;
		m_listCompositeEffect.push_back(new EffectComposite(m_currentEffCompId, *m_templateEffComposite));
	}
	printf("[msg] EffectCompositePool: Add %d EffectComposite to pool %d\n", iInitAmount, id);
}
EffectCompositePool::~EffectCompositePool() {
	delete m_templateEffComposite;
	for (int i = 0;i < m_listCompositeEffect.size();i++) {
		delete m_listCompositeEffect[i];
	}
}
EffectComposite* EffectCompositePool::GetCompositeEffect() {
	for (int i = 0;i < m_listCompositeEffect.size();i++) {
		if (!m_listCompositeEffect[i]->CheckIsActive()) {
			return m_listCompositeEffect[i];
		}
	}
	m_currentEffCompId++;
	m_listCompositeEffect.push_back(new EffectComposite(m_currentEffCompId ,*m_templateEffComposite));
	printf("[msg] EffectCompositePool: Add 1 EffectComposite to pool %d\n", m_id);
	return m_listCompositeEffect.back();
}

void EffectCompositePool::Update(float deltaTime) {
	for (int i = 0;i < m_listCompositeEffect.size();i++) {
		m_listCompositeEffect[i]->Update(deltaTime);
	}
}
void EffectCompositePool::Render(Camera2D* mainCamera) {
	for (int i = 0;i < m_listCompositeEffect.size();i++) {
		m_listCompositeEffect[i]->Render(mainCamera);
	}
}

void EffectCompositePool::DisableAll()
{
	for (int i = 0; i < m_listCompositeEffect.size(); i++) {
		m_listCompositeEffect[i]->SetActive(false);
	}
}
