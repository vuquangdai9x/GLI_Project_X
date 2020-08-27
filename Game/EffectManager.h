#pragma once
#include "Effect.h"
#include <vector>
#include "Singleton.h"
class EffectManager {
private:
	std::vector<Effect*> m_listEffect;
public:
	EffectManager();
	~EffectManager();
	void Update(float deltaTime);
	void Render();
	void CreateFirework(Vector2 position, int loopCount = 1, int typeId = -1);
};