#include "UIComponent.h"

void UIComponent::CalculateScaleForRenderType(Camera2D* mainCamera)
{
	Vector3 scaleSize(1, 1, 1);
	if (m_originSize.x != 0 && m_originSize.y != 0) {
		switch (m_renderType)
		{
		case RenderType::Fit:
			if ((m_right - m_left) / m_originSize.x * mainCamera->GetAspectRatio() < (m_top - m_bottom) / m_originSize.y) {
				scaleSize.x = (m_right - m_left) / 2;
				scaleSize.y = scaleSize.x / m_originSize.x * m_originSize.y * mainCamera->GetAspectRatio();
			}
			else {
				scaleSize.y = (m_top - m_bottom) / 2;
				scaleSize.x = scaleSize.y / m_originSize.y * m_originSize.x / mainCamera->GetAspectRatio();
			}
			break;
		case RenderType::Expand:
			if ((m_right - m_left) / m_originSize.x * mainCamera->GetAspectRatio() > (m_top - m_bottom) / m_originSize.y) {
				scaleSize.x = (m_right - m_left) / 2;
				scaleSize.y = scaleSize.x / m_originSize.x * m_originSize.y * mainCamera->GetAspectRatio();
			}
			else {
				scaleSize.y = (m_top - m_bottom) / 2;
				scaleSize.x = scaleSize.y / m_originSize.y * m_originSize.x / mainCamera->GetAspectRatio();
			}
			break;
		case RenderType::Stretch:
		default:
			scaleSize.x = (m_right - m_left) / 2;
			scaleSize.y = (m_top - m_bottom) / 2;
			break;
		}
	}
	m_scaleRenderMatrix.SetScale(scaleSize);
}

void UIComponent::CalculateTranslateForAlign(Camera2D* mainCamera)
{
	Vector3 translateAlign(0, 0, 0);
	m_translateAlignMatrix.SetTranslation(translateAlign);
}

UIComponent::UIComponent(int id)
	: Sprite(id), 
	m_renderType(RenderType::Fit), 
	m_alignH(AlignHorizontal::Middle), 
	m_alignV(AlignVertical::Center)
{
	m_top = 1;
	m_bottom = 0;
	m_left = 0;
	m_right = 1;
}
UIComponent::~UIComponent() {}

void UIComponent::Render(Camera2D* mainCamera) {
	if (m_material2d == NULL || m_model == NULL) return;
	if (m_originSize.x == 0 || m_originSize.y == 0) return;

	CalculateScaleForRenderType(mainCamera);
	CalculateTranslateForAlign(mainCamera);
	m_WVP = m_scaleRenderMatrix * m_translateAlignMatrix * m_transformMat;

	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_material2d->SetMainTexture(m_mainTexture);
	m_material2d->PrepareShader(m_WVP, Vector2(1,1), 0, 0, 1, 1, &m_color);

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

void UIComponent::SetBound(float top, float bottom, float left, float right) {
	m_top = top;
	m_bottom = bottom;
	m_left = left;
	m_right = right;
	m_position.x = (left + right) / 2;
	m_position.y = (top + bottom) / 2;
	m_T.SetTranslation(m_position);
	m_transformMat = m_S * m_R * m_T;
}
void UIComponent::SetRenderType(RenderType type) {
	m_renderType = type;
}
void UIComponent::SetAlignHorizontal(AlignHorizontal h) {
	m_alignH = h;
}
void UIComponent::SetAlignVertical(AlignVertical v) {
	m_alignV = v;
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