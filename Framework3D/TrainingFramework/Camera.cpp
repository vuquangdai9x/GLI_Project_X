#include "stdafx.h"
#include "Camera.h"

void Camera::UpdateViewMatrix()
{
	m_ViewMatrix.m[0][0] = m_xaxis.x;				m_ViewMatrix.m[0][1] = m_yaxis.x;				m_ViewMatrix.m[0][2] = m_zaxis.x;				m_ViewMatrix.m[0][3] = 0.0f;
	m_ViewMatrix.m[1][0] = m_xaxis.y;				m_ViewMatrix.m[1][1] = m_yaxis.y;				m_ViewMatrix.m[1][2] = m_zaxis.y;				m_ViewMatrix.m[1][3] = 0.0f;
	m_ViewMatrix.m[2][0] = m_xaxis.z;				m_ViewMatrix.m[2][1] = m_yaxis.z;				m_ViewMatrix.m[2][2] = m_zaxis.z;				m_ViewMatrix.m[2][3] = 0.0f;
	m_ViewMatrix.m[3][0] =-m_position.Dot(m_xaxis);	m_ViewMatrix.m[3][1] =-m_position.Dot(m_yaxis);	m_ViewMatrix.m[3][2] =-m_position.Dot(m_zaxis);	m_ViewMatrix.m[3][3] = 1.0f;
}

Camera::Camera()
{
	m_up = Vector3(0, 1, 0);
}

void Camera::Init(Vector3 position, Vector3 target, float nearPlane, float farPlane, float FOV, float aspectRatio, float rotateVerticalLimit)
{
	m_rotateLimit = rotateVerticalLimit;
	m_position = position;
	SetTarget(target);
	SetPerspective(FOV, aspectRatio, nearPlane, farPlane);
}

void Camera::SetPerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	m_ProjectionMatrix.SetPerspective(FOV, aspectRatio, nearPlane, farPlane);
}

void Camera::SetOrthorgraphic(float width, float height, float n, float f)
{
	if (width == 0 || height == 0 || f==n) return;
	float r = width / 2;
	float t = height / 2;
	m_ProjectionMatrix.m[0][0] = 1/r;	m_ProjectionMatrix.m[0][1] = 0;		m_ProjectionMatrix.m[0][2] = 0;			m_ProjectionMatrix.m[0][3] = 0;
	m_ProjectionMatrix.m[1][0] = 0;		m_ProjectionMatrix.m[1][1] = 1/t;	m_ProjectionMatrix.m[1][2] = 0;			m_ProjectionMatrix.m[1][3] = 0;
	m_ProjectionMatrix.m[2][0] = 0;		m_ProjectionMatrix.m[2][1] = 0;		m_ProjectionMatrix.m[2][2] = -2/(f-n);	m_ProjectionMatrix.m[2][3] = 0;
	m_ProjectionMatrix.m[3][0] = 0;		m_ProjectionMatrix.m[3][1] = 0;		m_ProjectionMatrix.m[3][2] =(f+n)/(n-f);m_ProjectionMatrix.m[3][3] = 1.0f;
}

void Camera::SetSpeed(float moveSpeedX, float moveSpeedY, float moveSpeedZ, float rotateSpeedHorizontal, float rotateSpeedVertical, float dutchSpeed)
{
	m_moveSpeedX = moveSpeedX;
	m_moveSpeedY = moveSpeedY;
	m_moveSpeedZ = moveSpeedZ;
	m_rotateSpeedHorizontal = rotateSpeedHorizontal;
	m_rotateSpeedVertical = rotateSpeedVertical;
	m_dutchSpeed = dutchSpeed;
}

void Camera::SetTarget(Vector3 newTargetPos)
{
	m_target = newTargetPos;
	m_zaxis = (m_position - m_target).Normalize();
	m_xaxis = (m_up.Cross(m_zaxis)).Normalize();
	m_yaxis = (m_zaxis.Cross(m_xaxis)).Normalize();
	UpdateViewMatrix();
}

Vector3& Camera::GetPosition()
{
	return m_position;
}

void Camera::MoveByLocalAxis(Vector3 localDirection, float deltaTime, float speedMultiplier)
{
	if (localDirection.Length() == 0) return;
	Vector3 deltaMove;
	deltaMove = (m_xaxis * localDirection.x) + (m_yaxis * localDirection.y) + (-m_zaxis * localDirection.z);
	deltaMove = deltaMove.Normalize() * speedMultiplier * deltaTime;
	deltaMove.x *= m_moveSpeedX;
	deltaMove.y *= m_moveSpeedY;
	deltaMove.z *= m_moveSpeedZ;

	m_position += deltaMove;
	m_target += deltaMove;
	UpdateViewMatrix();
}

void Camera::MoveByWorldAxis(Vector3 worldDirection, float deltaTime, float speedMultiplier)
{
	if (worldDirection.Length() == 0) return;
	Vector3 deltaMove = worldDirection.Normalize() * speedMultiplier * deltaTime;
	deltaMove.x *= m_moveSpeedX;
	deltaMove.y *= m_moveSpeedY;
	deltaMove.z *= m_moveSpeedZ;

	m_position += deltaMove; 
	m_target += deltaMove;
	UpdateViewMatrix();
}

void Camera::Rotate(float horizontal, float vertical, float deltaTime, float speedMultiplier)
{
	if (vertical == 0 && horizontal == 0) return;
	float length = sqrt(vertical * vertical + horizontal * horizontal);
	vertical = vertical / length * m_rotateSpeedVertical * speedMultiplier * deltaTime;
	horizontal = horizontal / length * m_rotateSpeedHorizontal * speedMultiplier * deltaTime;

	// limit up & down rotation
	Vector3 worldDeltaPosCameraTarget = m_target - m_position;
	float currAngle = acos(m_up.Dot(worldDeltaPosCameraTarget) / (worldDeltaPosCameraTarget.Length()));
	if ((currAngle + vertical < 0 + m_rotateLimit) || (currAngle + vertical > M_PI - m_rotateLimit)) {
		vertical = 0;
	}

	Vector4 localTargetPosition(0, 0, (m_position - m_target).Length(), 1);
	Vector4 newLocalTargetPosition = localTargetPosition * Matrix().SetRotationX(vertical) * Matrix().SetRotationY(horizontal);
	Vector3 newTarget = m_position + (m_xaxis*newLocalTargetPosition.x + m_yaxis*newLocalTargetPosition.y - m_zaxis*newLocalTargetPosition.z);
	SetTarget(newTarget);
}

void Camera::SetVectorUp(Vector3 value)
{
	m_up = value;
	SetTarget(m_target); // set target again to recalculate x,y,z-axis
}

void Camera::Dutch(float angle, float deltaTime, float speedMultiplier)
{
	if (angle == 0) return;
	angle = (angle>0?1:-1) * m_dutchSpeed * deltaTime * speedMultiplier;

	Vector4 localUp(0, 0, 0, 0);
	localUp.z = m_up.Dot(m_zaxis);
	localUp.y = sqrt(1 - localUp.z * localUp.z);
	localUp = localUp * Matrix().SetRotationZ(angle);
	m_up = m_xaxis * localUp.x + m_yaxis * localUp.y + m_zaxis * localUp.z;
	SetTarget(m_target); // set target again to recalculate x,y,z-axis
}

Matrix & Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

Matrix & Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}
