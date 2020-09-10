#pragma once
#include "Effect.h"
#include <vector>
class EffectComposite {
private:
	int m_id;
	std::vector<Effect*> m_listEffect;
	bool m_isActive;
	Vector3 m_position;
public:
	EffectComposite(int id);
	EffectComposite(int id, EffectComposite& ec);
	~EffectComposite();
	int GetId();
	bool AddEffect(Effect* effect);

	void Play(Vector3 position, Vector2 scale, float rotation);

	void Update(float deltaTime);
	void Render(Camera2D* camera);

	void SetActive(bool value);
	bool CheckIsActive();
};