#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
class Effect {
protected:
	Vector3 m_position;
	int m_iLoopCount;
	bool m_isEnabled;
public:
	Effect(Vector3 position, int loopCount = 1);
	virtual ~Effect();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void SetEnable(bool value);
	bool GetEnaled();
	int GetLoopCount
};