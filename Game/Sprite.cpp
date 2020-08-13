#include "stdafx.h"
#include "Sprite.h"
#include "ResourcesManager2D.h"
#include "Singleton.h"

Sprite::Sprite()
{
}

Sprite::Sprite(int id)
{
	m_id = id;
}

void Sprite::Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId) {
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);
	SetColor(hexColor, alpha);
	m_canRender = true;
	m_model = Singleton<ResourceManager2D>::GetInstance()->GetModel(0);
	if (m_model == NULL) {
		m_canRender = false;
		printf("[ERR] GameObject: Failed to get model %d\n", 0);
	}
	m_material2d = dynamic_cast<Material2D*>(Singleton<ResourceManager2D>::GetInstance()->GetMaterial(iMaterialId));
	if (m_material2d == NULL) {
		m_canRender = false;
		printf("[ERR] Sprite2d: Can't get Material2D\n");
	}
	m_mainTexture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iMainTexId);
	if (m_mainTexture == NULL) {
		printf("[ERR] GameObject: Failed to get main texture %d\n", iMainTexId);
	}
}

void Sprite::Update(float deltaTime) {	

}
int glhProjectf(float objx, float objy, float objz, Matrix modelview, Matrix projection, int* viewport, float* windowCoordinate)
{
	// Transformation vectors
	float fTempo[8];
	// Modelview transform
	fTempo[0] = modelview.m[0][0] * objx + modelview.m[1][0] * objy + modelview.m[2][0] * objz + modelview.m[3][0]; // w is always 1
	fTempo[1] = modelview.m[0][1] * objx + modelview.m[1][1] * objy + modelview.m[2][1] * objz + modelview.m[3][1];
	fTempo[2] = modelview.m[0][2] * objx + modelview.m[1][2] * objy + modelview.m[2][2] * objz + modelview.m[3][2];
	fTempo[3] = modelview.m[0][3] * objx + modelview.m[1][3] * objy + modelview.m[2][3] * objz + modelview.m[3][3];
	// Projection transform, the final row of projection matrix is always [0 0 -1 0]
	// so we optimize for that.
	fTempo[4] = projection.m[0][0] * fTempo[0] + projection.m[1][0] * fTempo[1] + projection.m[2][0] * fTempo[2] + projection.m[3][0] * fTempo[3];
	fTempo[5] = projection.m[0][1] * fTempo[0] + projection.m[1][1] * fTempo[1] + projection.m[2][1] * fTempo[2] + projection.m[3][1] * fTempo[3];
	fTempo[6] = projection.m[0][2] * fTempo[0] + projection.m[1][2] * fTempo[1] + projection.m[2][2] * fTempo[2] + projection.m[3][2] * fTempo[3];
	fTempo[7] = projection.m[0][3] * fTempo[0] + projection.m[1][3] * fTempo[1] + projection.m[2][3] * fTempo[2] + projection.m[3][3] * fTempo[3];
	// The result normalizes between -1 and 1
	if (fTempo[7] == 0.0) // The w value
		return 0;
	fTempo[7] = 1.0 / fTempo[7];
	// Perspective division
	fTempo[4] *= fTempo[7];
	fTempo[5] *= fTempo[7];
	fTempo[6] *= fTempo[7];
	// Window coordinates
	// Map x, y to range 0-1
	windowCoordinate[0] = (fTempo[4] * 0.5 + 0.5) * viewport[2] + viewport[0];
	windowCoordinate[1] =  (1-(fTempo[5] * 0.5 + 0.5)) * viewport[3] + viewport[1];
	// This is only correct when glDepthRange(0.0, 1.0)
	windowCoordinate[2] = (1.0 + fTempo[6]) * 0.5;	// Between 0 and 1
	return 1;
}

int glhUnProjectf(float winx, float winy, float winz, Matrix modelview, Matrix projection, int* viewport, float* objectCoordinate)
{
	// Transformation matrices
	Matrix m_matrix, a_matrix;
	Vector4 v_in, v_out;
	a_matrix = modelview * projection;
	// Now compute the inverse of matrix A
	if (a_matrix.InvertMatrix(m_matrix) == 0)
		return 0;
	v_in.x = (winx - (float)viewport[0]) / (float)viewport[2] * 2.0 - 1.0;
	v_in.y = (winy - (float)viewport[1]) / (float)viewport[3] * 2.0 - 1.0;
	v_in.z = 2.0 * winz - 1.0;
	v_in.w = 1.0;
	// Objects coordinates
	v_out = v_in * m_matrix;
	if (v_out.w == 0.0)
		return 0;
	v_out.w = 1.0 / v_out.w;
	objectCoordinate[0] = v_out.x * v_out.w;
	objectCoordinate[1] = -v_out.y * v_out.w;
	objectCoordinate[2] = v_out.z * v_out.w;
	return 1;
}


void Sprite::Render(Camera2D* mainCamera) {
	if (!m_canRender) return;
	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();
	//m_WVP = m_transformMat;
	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_material2d->SetMainTexture(m_mainTexture);
	m_material2d->PrepareShader(m_WVP, m_mainTexture->GetRatio(), 0,0,1,1,&m_color);

	glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Sprite::OnDestroy() {

}

void Sprite::UpdateRotationMatrix()
{
	Matrix rx, ry, rz;
	m_R = rz.SetRotationZ(m_rotation) * rx.SetRotationX(0) * ry.SetRotationY(0);
	m_transformMat = m_S * m_R * m_T;
}

void Sprite::UpdateTranslateMatrix()
{
	m_T.SetTranslation(m_position);
	m_transformMat = m_S * m_R * m_T;
}

void Sprite::UpdateScaleMatrix()
{
	m_S.SetScale(Vector3(m_scale.x,m_scale.y,1));
	m_transformMat = m_S * m_R * m_T;
}

int Sprite::GetId()
{
	return m_id;
}

void Sprite::SetRotation(float rotation)
{
	m_rotation = rotation;
	UpdateRotationMatrix();
}

float Sprite::GetRotation()
{
	return m_rotation;
}

void Sprite::SetPosition(Vector3 position)
{
	m_position = position;
	UpdateTranslateMatrix();
}

Vector3 Sprite::GetPosition()
{
	return m_position;
}

void Sprite::SetScale(Vector2 scale)
{
	m_scale = scale;
	UpdateScaleMatrix();
}

Vector2 Sprite::GetScale()
{
	return m_scale;
}

void Sprite::SetColor(unsigned int hexColor, float alpha) {
	m_color.x = ((hexColor & 0xFF0000) >> 16) / (float)255;
	m_color.y = ((hexColor & 0x00FF00) >> 8) / (float)255;
	m_color.z = ((hexColor & 0x0000FF) >> 0) / (float)255;
	m_color.w = alpha;
}

void Sprite::SetColor(Vector4 color) {
	m_color.x = color.x;
	m_color.y = color.y;
	m_color.z = color.z;
	m_color.w = color.w;
}

Vector4 Sprite::GetColor() {
	return m_color;
}