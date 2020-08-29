#include "Effect.h"

Effect::Effect(Vector3 position, Vector2 scale, float rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
	m_isEnabled = false;
}

Effect::Effect(Effect& effect){
	m_position = effect.m_position;
	m_scale = effect.m_scale;
	m_rotation = effect.m_rotation;
	m_WVP = effect.m_WVP;
	m_transformMat = effect.m_transformMat;
	m_T = effect.m_T;
	m_R = effect.m_R;
	m_S = effect.m_S;
	m_isEnabled = false;
}

Effect::~Effect()
{}

//int Effect::GetId()
//{
//	return m_id;
//}

void Effect::SetActive(bool value)
{
	m_isEnabled = value;
}

bool Effect::CheckIsActive()
{
	return m_isEnabled;
}

void Effect::UpdateRotationMatrix()
{
	Matrix rx, ry, rz;
	m_R = rz.SetRotationZ(m_rotation) * rx.SetRotationX(0) * ry.SetRotationY(0);
	m_transformMat = m_S * m_R * m_T;
}

void Effect::UpdateTranslateMatrix()
{
	m_T.SetTranslation(m_position);
	m_transformMat = m_S * m_R * m_T;
}

void Effect::UpdateScaleMatrix()
{
	m_S.SetScale(Vector3(m_scale.x, m_scale.y, 1));
	m_transformMat = m_S * m_R * m_T;
}

void Effect::SetRotation(float rotation)
{
	m_rotation = rotation;
	UpdateRotationMatrix();
}

float Effect::GetRotation()
{
	return m_rotation;
}

void Effect::SetPosition(Vector3 position)
{
	m_position = position;
	UpdateTranslateMatrix();
}

Vector3 Effect::GetPosition()
{
	return m_position;
}

void Effect::SetScale(Vector2 scale)
{
	m_scale = scale; 
	UpdateScaleMatrix();
}

Vector2 Effect::GetScale()
{
	return m_scale;
}