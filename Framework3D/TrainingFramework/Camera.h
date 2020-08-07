#pragma once
#include "stdafx.h"

class Camera {
private:
	Matrix m_ProjectionMatrix, m_ViewMatrix;
	Vector3 m_up, m_xaxis, m_yaxis, m_zaxis;

	Vector3 m_target;
	Vector3 m_position;
	float m_rotateLimit = M_PI/2; // determine what angle with UP vector that can't rotate to (radian)

	float m_moveSpeedX = 0, m_moveSpeedY = 0, m_moveSpeedZ = 0; // m/s
	float m_rotateSpeedHorizontal = 0, m_rotateSpeedVertical = 0; // rad/s
	float m_dutchSpeed = 0; //rad/s

	void UpdateViewMatrix();
public:
	Camera();

	void Init(Vector3 position, Vector3 target, float nearPlane, float farPlane, float FOV, float aspectRatio, float rotateVerticalLimit);
	void SetPerspective(float FOV, float aspectRatio, float nearPlane, float farPlane);
	void SetOrthorgraphic(float width, float height, float nearPlane, float farPlane);
	void SetSpeed(float moveXSpeed, float moveYSpeed, float moveZSpeed, float rotateHorizontalSpeed, float rotateVerticalSpeed, float dutchSpeed);
	void SetTarget(Vector3 newTargetPos);

	void MoveByLocalAxis(Vector3 localDirection, float deltaTime, float speedMultiplier = 1);
	void MoveByWorldAxis(Vector3 worldDirection, float deltaTime, float speedMultiplier = 1);
	void Rotate(float horizontal, float vertical, float deltaTime, float speedMultiplier = 1);

	void SetVectorUp(Vector3 value);
	void Dutch(float angle, float deltaTime, float speedMultiplier = 1);

	Vector3& GetPosition();
	Matrix & GetViewMatrix();
	Matrix & GetProjectionMatrix();
};