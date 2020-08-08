#include "stdafx.h"
#include "Sprite.h"
#include "ResourcesManager2D.h"

Sprite::Sprite(int id)
{
	m_id = id;
}

void Sprite::Init(Vector3 position, float rotation, Vector2 scale,  int iMaterialId, int iMainTexId) {
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);
	m_canRender = true;
	m_model = ResourceManager2D::GetInstance()->GetModel(0);
	if (m_model == NULL) {
		m_canRender = false;
		printf("[ERR] GameObject: Failed to get model %d\n", 0);
	}
	m_material2d = dynamic_cast<Material2D*>(ResourceManager2D::GetInstance()->GetMaterial(iMaterialId));
	if (m_material2d == NULL) {
		m_canRender = false;
		printf("[ERR] Sprite2d: Can't get Material2D\n");
	}
	m_mainTexture = ResourceManager2D::GetInstance()->GetTexture(iMainTexId);
	if (m_mainTexture == NULL) {
		printf("[ERR] GameObject: Failed to get main texture %d\n", iMainTexId);
	}
}

void Sprite::Update(float deltaTime) {

}

void Sprite::Render(Camera2D* mainCamera) {
	if (!m_canRender) return;
	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();
	//m_WVP = m_transformMat;
	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_material2d->SetMainTexture(m_mainTexture);
	m_material2d->PrepareShader(m_WVP, 0,0,1,1);

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