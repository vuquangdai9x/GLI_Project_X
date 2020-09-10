#pragma once
#include "EffectComposite.h"
#include <vector>
class EffectCompositePool {
private:
	int m_id;
	int m_currentEffCompId;
	EffectComposite* m_templateEffComposite;
	std::vector<EffectComposite*> m_listCompositeEffect;
public:
	int GetId();
	EffectCompositePool(int id, int iInitAmount, EffectComposite* templateEffComposite);
	virtual ~EffectCompositePool();

	EffectComposite* GetCompositeEffect();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
	void DisableAll();
};