#include "UIComponent.h"

UIComponent::UIComponent(int id):Sprite(id), m_renderType(RenderType::Fit)
{
	m_top = 1;
	m_bottom = 0;
	m_left = 0;
	m_right = 1;
}
UIComponent::~UIComponent() {}

void UIComponent::Render(Camera2D* mainCamera) {
	if (!m_canRender) return;
	m_WVP = m_transformMat;

	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_material2d->SetMainTexture(m_mainTexture);
	m_material2d->PrepareShader(m_WVP, m_originSize, 0, 0, 1, 1, &m_color);

	glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void UIComponent::SetPosition(Vector3 position) {
	Vector2 delta;
	delta.x = position.x - m_position.x;
	delta.y = position.y - m_position.y;
	Sprite::SetPosition(position);

	m_left += delta.x;
	m_right += delta.x;
	m_top += delta.y;
	m_bottom += delta.y;
}
void UIComponent::SetScale(Vector2 scale) {
	Sprite::SetScale(scale);
	m_left = m_position.x - (m_scale.x * m_originSize.x) / 2;
	m_right = m_position.x + (m_scale.x * m_originSize.x) / 2;
	m_top = m_position.y - (m_scale.y * m_originSize.y) / 2;
	m_bottom = m_position.y - (m_scale.y * m_originSize.y) / 2;
	printf("[Wrn] UIComponent: Scale of UIComponent should not be set directly\n");
}

void UIComponent::SetBound(float top, float bottom, float left, float right) {
	m_top = top;
	m_bottom = bottom;
	m_left = left;
	m_right = right;
	m_position.x = (left + right) / 2;
	m_position.y = (top + bottom) / 2;
	if (m_originSize.x != 0 && m_originSize.y != 0) {
		switch (m_renderType)
		{
		case RenderType::Fit:
			if ((right - left) / m_originSize.x < (top - bottom) / m_originSize.y) {
				m_scale.x = m_scale.y = (right - left) / m_originSize.x / 2;
			}
			else {
				m_scale.x = m_scale.y = (top - bottom) / m_originSize.y / 2;
			}
			break;
		case RenderType::Expand:
			if ((right - left) / m_originSize.x > (top - bottom) / m_originSize.y) {
				m_scale.x = m_scale.y = (right - left) / m_originSize.x / 2;
			}
			else {
				m_scale.x = m_scale.y = (top - bottom) / m_originSize.y / 2;
			}
			break;
		case RenderType::Stretch:
		default:
			m_scale.x = (right - left) / m_originSize.x / 2;
			m_scale.y = (top - bottom) / m_originSize.y / 2;
			break;
		}
	}
	m_T.SetTranslation(m_position);
	m_S.SetScale(Vector3(m_scale.x, m_scale.y, 1));
	m_transformMat = m_S * m_R * m_T;
}
void UIComponent::SetRenderType(RenderType type) {
	if (m_renderType == type) return;
	m_renderType = type;
	if (m_originSize.x != 0 && m_originSize.y != 0) {
		switch (m_renderType)
		{
		case RenderType::Fit:
			if ((m_right - m_left) / m_originSize.x < (m_top - m_bottom) / m_originSize.y) {
				m_scale.x = m_scale.y = (m_right - m_left) / m_originSize.x / 2;
			}
			else {
				m_scale.x = m_scale.y = (m_top - m_bottom) / m_originSize.y / 2;
			}
			break;
		case RenderType::Expand:
			if ((m_right - m_left) / m_originSize.x > (m_top - m_bottom) / m_originSize.y) {
				m_scale.x = m_scale.y = (m_right - m_left) / m_originSize.x / 2;
			}
			else {
				m_scale.x = m_scale.y = (m_top - m_bottom) / m_originSize.y / 2;
			}
			break;
		case RenderType::Stretch:
		default:
			m_scale.x = (m_right - m_left) / m_originSize.x / 2;
			m_scale.y = (m_top - m_bottom) / m_originSize.y / 2;
			break;
		}
	}
	m_S.SetScale(Vector3(m_scale.x, m_scale.y, 1));
	m_transformMat = m_S * m_R * m_T;
}

void UIComponent::SetTop(float value) {
	SetBound(value, m_bottom, m_left, m_right);
}
float UIComponent::GetTop() {
	return m_top;
}
void UIComponent::SetBottom(float value) {
	SetBound(m_top, value, m_left, m_right);
}
float UIComponent::GetBottom() {
	return m_bottom;
}
void UIComponent::SetLeft(float value) {
	SetBound(m_top, m_bottom, value, m_right);
}
float UIComponent::GetLeft() {
	return m_left;
}
void UIComponent::SetRight(float value) {
	SetBound(m_top, m_bottom, m_left, value);
}
float UIComponent::GetRight() {
	return m_right;
}