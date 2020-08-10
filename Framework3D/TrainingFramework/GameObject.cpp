#include "stdafx.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Singleton.h"

void GameObject::UpdateRotationMatrix()
{
	Matrix rx, ry, rz;
	m_R = rz.SetRotationZ(m_rotation.z)*rx.SetRotationX(m_rotation.x)*ry.SetRotationY(m_rotation.y);
	m_transformMat = m_S*m_R*m_T;
}

void GameObject::UpdateTranslateMatrix()
{
	m_T.SetTranslation(m_position);
	m_transformMat = m_S*m_R*m_T;
}

void GameObject::UpdateScaleMatrix()
{
	m_S.SetScale(m_scale);
	m_transformMat = m_S*m_R*m_T;
}

GameObject::GameObject(int id)
{
	m_id = id;
}

int GameObject::GetId()
{
	return m_id;
}

void GameObject::Init()
{
	
}

void GameObject::Init(Vector3 position, Vector3 rotation, Vector3 scale, int iModelId, int iMaterialId, int iMainTextureId)
{
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);

	m_canRender = true;
	m_model = Singleton<ResourceManager>::GetInstance()->GetModel(iModelId);
	if (m_model == NULL) {
		m_canRender = false;
		printf("[ERR] GameObject: Failed to get model %d\n", iModelId);
	}
	m_material = Singleton<ResourceManager>::GetInstance()->GetMaterial(iMaterialId);
	if (m_material == NULL) {
		m_canRender = false;
		printf("[ERR] GameObject: Failed to get material %d\n", iMaterialId);
	}
	m_mainTexture = Singleton<ResourceManager>::GetInstance()->GetTexture(iMainTextureId);
	if (m_mainTexture == NULL) {
		printf("[ERR] GameObject: Failed to get main texture %d\n", iMainTextureId);
	}
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Render(Camera * mainCamera)
{
	if (!m_canRender) return;
	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();
	//m_WVP = m_transformMat;
	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_material->SetMainTexture(m_mainTexture);
	m_material->PrepareShader(m_WVP);

	glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::OnDestroy()
{
}

void GameObject::Rotate(Vector3 deltaRotation)
{
	m_rotation += deltaRotation;
	UpdateRotationMatrix();
}

void GameObject::SetRotation(Vector3 rotation)
{
	m_rotation = rotation;
	UpdateRotationMatrix();
}

Vector3 & GameObject::GetRotation()
{
	return m_rotation;
}

void GameObject::MoveByWorldAxis(Vector3 deltaMove)
{
	m_position += deltaMove;
	UpdateTranslateMatrix();
}

void GameObject::SetPosition(Vector3 position)
{
	m_position = position;
	UpdateTranslateMatrix();
}

Vector3 & GameObject::GetPosition()
{
	return m_position;
}

void GameObject::SetScale(Vector3 scale)
{
	m_scale = scale;
	UpdateScaleMatrix();
}

Vector3 & GameObject::GetScale()
{
	return m_scale;
}