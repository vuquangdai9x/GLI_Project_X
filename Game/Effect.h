#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
#include <vector>
class Effect {
protected:
	//int m_id;
	bool m_isEnabled;

	Vector3 m_position;
	Vector2 m_scale;
	float m_rotation;

	Matrix m_WVP;
	Matrix m_transformMat, m_T, m_R, m_S;
	void UpdateRotationMatrix();
	void UpdateTranslateMatrix();
	void UpdateScaleMatrix();

public:
	Effect(Vector3 position, Vector2 scale, float rotation);
	Effect(Effect& effect);
	//Effect(int id);
	//Effect(int id, Effect& effect);
	virtual ~Effect();
	//int GetId();
	virtual void Play() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Camera2D* mainCamera) = 0;
	void SetActive(bool value);
	bool CheckIsActive();

	virtual void SetRotation(float rotation);
	float GetRotation();
	virtual void SetPosition(Vector3 position);
	Vector3 GetPosition();
	virtual void SetScale(Vector2 scale);
	Vector2 GetScale();
};