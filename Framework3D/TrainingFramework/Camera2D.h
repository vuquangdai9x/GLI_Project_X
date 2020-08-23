#pragma once
#include "stdafx.h"
class Camera2D{
private:
	Matrix m_ProjectionMatrix, m_ViewMatrix;
	Vector3 m_up, m_xaxis, m_yaxis, m_zaxis;

	Vector3 m_position;
	float m_dutchAngle;
	float m_zoom;
	float m_aspectRatio;
	float m_nearPlane, m_farPlane;

	Vector3 m_moveSpeed;
	float m_dutchSpeed;
	float m_zoomSpeed;

	void UpdateViewMatrix();
public:
	Camera2D();

	void Update(float deltaTime);

	void Init(Vector3 position, float dutchAngle);
	void SetPerspective(float FOV, float aspectRatio, float nearPlane, float farPlane);
	void SetOrthorgraphic(float zoom, float aspectRatio, float n, float f);

	Vector3& GetPosition();
	void SetPosition(Vector3 position);
	void Move(Vector3 direction, float deltaTime, float speedMultiplier = 1);
	
	float GetDutchAngle();
	void SetDutchAngle(float angle);
	void Dutch(float direction, float deltaTime, float speedMultiplier = 1);

	float GetZoom();
	void SetZoom(float value);
	void Zoom(float direction, float deltaTime, float speedMultiplier = 1);

	float GetNearPlane();
	float GetFarPlane();
	float GetAspectRatio();
	
	Matrix& GetViewMatrix();
	Matrix& GetProjectionMatrix();
};