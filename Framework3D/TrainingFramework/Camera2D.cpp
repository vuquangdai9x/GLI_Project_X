#include "stdafx.h"
#include "Camera2D.h"

void Camera2D::UpdateViewMatrix()
{
	m_ViewMatrix.m[0][0] = m_xaxis.x;				m_ViewMatrix.m[0][1] = m_yaxis.x;				m_ViewMatrix.m[0][2] = m_zaxis.x;				m_ViewMatrix.m[0][3] = 0.0f;
	m_ViewMatrix.m[1][0] = m_xaxis.y;				m_ViewMatrix.m[1][1] = m_yaxis.y;				m_ViewMatrix.m[1][2] = m_zaxis.y;				m_ViewMatrix.m[1][3] = 0.0f;
	m_ViewMatrix.m[2][0] = m_xaxis.z;				m_ViewMatrix.m[2][1] = m_yaxis.z;				m_ViewMatrix.m[2][2] = m_zaxis.z;				m_ViewMatrix.m[2][3] = 0.0f;
	m_ViewMatrix.m[3][0] = -m_position.Dot(m_xaxis);	m_ViewMatrix.m[3][1] = -m_position.Dot(m_yaxis);	m_ViewMatrix.m[3][2] = -m_position.Dot(m_zaxis);	m_ViewMatrix.m[3][3] = 1.0f;
}

Camera2D::Camera2D()
{ 
	m_up = Vector3(0, 1, 0);
	m_zaxis = Vector3(0, 0, -1);
	m_xaxis = (m_up.Cross(m_zaxis)).Normalize();
	m_yaxis = (m_zaxis.Cross(m_xaxis)).Normalize();

	m_dutchSpeed = 30 * 2 * M_PI / 360;
	m_moveSpeed = Vector3(5,5,0);
	m_zoomSpeed = 2;
}

void Camera2D::Init(Vector3 position, float dutchAngle)
{
	SetPosition(position);
	SetDutchAngle(dutchAngle);
}

void Camera2D::SetPerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	m_zoom = FOV;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_aspectRatio = aspectRatio;
	m_ProjectionMatrix.SetPerspective(FOV, aspectRatio, nearPlane, farPlane);
}

void Camera2D::SetOrthorgraphic(float zoom, float aspectRatio, float n, float f)
{
	if (zoom == 0 || aspectRatio == 0 || f == n) return;
	m_zoom = zoom;
	m_nearPlane = n;
	m_farPlane = f;
	m_aspectRatio = aspectRatio;
	float r = zoom * aspectRatio / 2;
	float t = zoom / 2;
	m_ProjectionMatrix.m[0][0] = -1 / r;	m_ProjectionMatrix.m[0][1] = 0;			m_ProjectionMatrix.m[0][2] = 0;					m_ProjectionMatrix.m[0][3] = 0;
	m_ProjectionMatrix.m[1][0] = 0;			m_ProjectionMatrix.m[1][1] = 1 / t;	m_ProjectionMatrix.m[1][2] = 0;					m_ProjectionMatrix.m[1][3] = 0;
	m_ProjectionMatrix.m[2][0] = 0;			m_ProjectionMatrix.m[2][1] = 0;			m_ProjectionMatrix.m[2][2] = -2 / (f - n);		m_ProjectionMatrix.m[2][3] = 0;
	m_ProjectionMatrix.m[3][0] = 0;			m_ProjectionMatrix.m[3][1] = 0;			m_ProjectionMatrix.m[3][2] = (f + n) / (n - f);	m_ProjectionMatrix.m[3][3] = 1.0f;
}

Vector3& Camera2D::GetPosition()
{
	return m_position;
}

void Camera2D::SetPosition(Vector3 position) {
	m_position = position;
	UpdateViewMatrix();
}

void Camera2D::Move(Vector3 direction, float deltaTime, float speedMultiplier)
{
	if (direction.x == 0 && direction.y==0 && direction.z==0) return;
	Vector3 deltaMove = direction.Normalize() * speedMultiplier * deltaTime;
	deltaMove.x *= m_moveSpeed.x;
	deltaMove.y *= m_moveSpeed.y;
	deltaMove.z = 0;

	SetPosition(m_position + deltaMove);
}

float Camera2D::GetDutchAngle() {
	return m_dutchAngle;
}

void Camera2D::SetDutchAngle(float angle) {
	m_dutchAngle = angle;
	m_up = Vector3(cosf(m_dutchAngle + M_PI / 2), sinf(m_dutchAngle + M_PI / 2), 0);
	m_xaxis = (m_up.Cross(m_zaxis)).Normalize();
	m_yaxis = (m_zaxis.Cross(m_xaxis)).Normalize();
	UpdateViewMatrix();
}

void Camera2D::Dutch(float direction, float deltaTime, float speedMultiplier)
{
	if (direction == 0) return;
	float deltaAngle = (direction > 0 ? 1 : -1) * m_dutchSpeed * deltaTime * speedMultiplier;
	SetDutchAngle(m_dutchAngle + deltaAngle);
}

float Camera2D::GetZoom() {
	return m_zoom;
}
void Camera2D::SetZoom(float value) {
	if (value <= 0) return;
	m_zoom = value;
	SetOrthorgraphic(m_zoom, m_aspectRatio, m_nearPlane, m_farPlane);
}
void Camera2D::Zoom(float direction, float deltaTime, float speedMultiplier) {
	if (direction == 0) return;
	float deltaZoom = (direction > 0 ? 1 : -1) * m_zoomSpeed * deltaTime * speedMultiplier;
	SetZoom(m_zoom + deltaZoom);
}

float Camera2D::GetNearPlane() { return m_nearPlane; }
float Camera2D::GetFarPlane() { return m_farPlane; }

float Camera2D::GetAspectRatio()
{
	return m_aspectRatio;
}

Matrix & Camera2D::GetViewMatrix()
{
	return m_ViewMatrix;
}

Matrix& Camera2D::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

void Camera2D::Update(float deltaTime) {
	//printf("Camera: position=%f,%f,%f | zoom=%f | dutch=%f\n", m_position.x, m_position.y, m_position.z,m_zoom,m_dutchAngle);
}
