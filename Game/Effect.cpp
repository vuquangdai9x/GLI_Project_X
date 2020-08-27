#include "Effect.h"

Effect::Effect(Vector3 position, int loopCount) : m_position(position), m_iLoopCount(loopCount) 
{
	m_isEnabled = true;
}

Effect::~Effect()
{}

void Effect::Update(float deltaTime) {
	if (!m_isEnabled) return;
}

void Effect::Render() {
	if (!m_isEnabled) return;
}

void Effect::SetEnable(bool value)
{
	m_isEnabled = value;
}

bool Effect::GetEnaled()
{
	return m_isEnabled;
}
