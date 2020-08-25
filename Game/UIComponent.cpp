#include "UIComponent.h"

void UIComponent::CalculateUITransform(Camera2D* mainCamera)
{
	float originWidth = 0, originHeight = 0;
	if (m_isUseAnimation) {
		originWidth = m_animationController.m_frameWidth;
		originHeight = m_animationController.m_frameHeight;
	}
	else {
		originWidth = m_originSize.x;
		originHeight = m_originSize.y;
	}
	if (originWidth == 0 || originHeight == 0) return;

	Vector3 scaleSize(1, 1, 1);
	Vector3 translateAlign(0, 0, 0);
	bool isAlignH = false, isAlignV = false;
	switch (m_renderType)
	{
	case RenderType::Fit:
		if ((m_right - m_left) / originWidth * mainCamera->GetAspectRatio() < (m_top - m_bottom) / originHeight) {
			scaleSize.x = (m_right - m_left) / 2;
			scaleSize.y = scaleSize.x / originWidth * originHeight * mainCamera->GetAspectRatio();
			isAlignV = true;
		}
		else {
			scaleSize.y = (m_top - m_bottom) / 2;
			scaleSize.x = scaleSize.y / originHeight * originWidth / mainCamera->GetAspectRatio();
			isAlignH = true;
		}
		break;
	case RenderType::Expand:
		if ((m_right - m_left) / originWidth * mainCamera->GetAspectRatio() > (m_top - m_bottom) / originHeight) {
			scaleSize.x = (m_right - m_left) / 2;
			scaleSize.y = scaleSize.x / originWidth * originHeight * mainCamera->GetAspectRatio();
			isAlignV = true;
		}
		else {
			scaleSize.y = (m_top - m_bottom) / 2;
			scaleSize.x = scaleSize.y / originHeight * originWidth / mainCamera->GetAspectRatio();
			isAlignH = true;
		}
		break;
	case RenderType::FitHeight:
		scaleSize.y = (m_top - m_bottom) / 2;
		scaleSize.x = scaleSize.y / originHeight * originWidth / mainCamera->GetAspectRatio();
		isAlignH = true;
		break;
	case RenderType::FitWidth:
		scaleSize.x = (m_right - m_left) / 2;
		scaleSize.y = scaleSize.x / originWidth * originHeight * mainCamera->GetAspectRatio();
		isAlignV = true;
		break;
	case RenderType::Stretch:
	default:
		scaleSize.x = (m_right - m_left) / 2;
		scaleSize.y = (m_top - m_bottom) / 2;
		break;
	}

	if (isAlignV) {
		switch (m_alignV)
		{
		case AlignVertical::Top:
			translateAlign.y = (m_top - m_position.y) - scaleSize.y;
			break;
		case AlignVertical::Bottom:
			translateAlign.y = -((m_position.y - m_bottom) - scaleSize.y);
			break;
		case AlignVertical::Middle:
		default:
			translateAlign.y = 0;
			break;
		}
		//if (m_renderType == RenderType::Expand) translateAlign.y = -translateAlign.y;
	}
	if (isAlignH) {
		switch (m_alignH)
		{
		case AlignHorizontal::Left:
			translateAlign.x = -((m_position.x - m_left) - scaleSize.x);
			break;
		case AlignHorizontal::Right:
			translateAlign.x = (m_right - m_position.x) - scaleSize.x;
			break;
		case AlignHorizontal::Center:
		default:
			translateAlign.x = 0;
			break;
		}
		//if (m_renderType == RenderType::Expand) translateAlign.x = -translateAlign.x;
	}

	

	m_scaleUIMatrix = Matrix().SetScale(scaleSize);
	m_translateAlignMatrix = Matrix().SetTranslation(translateAlign);
}

UIComponent::UIComponent(int id)
	: Sprite(id), 
	m_renderType(RenderType::Fit), 
	m_alignH(AlignHorizontal::Center), 
	m_alignV(AlignVertical::Middle)
{
	m_top = 1;
	m_bottom = 0;
	m_left = 0;
	m_right = 1;
}
UIComponent::~UIComponent() {}

void UIComponent::Render(Camera2D* mainCamera) {
	if (m_material2d == NULL || m_model == NULL) return;
	
	CalculateUITransform(mainCamera);
	m_WVP = m_scaleUIMatrix * m_transformMat * m_translateAlignMatrix;

	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	if (m_isUseAnimation) {
		m_material2d->SetMainTexture(m_animationController.GetTexture());
		m_material2d->PrepareShader(m_WVP,
			1, 1,
			m_animationController.m_offsetX,
			m_animationController.m_offsetY,
			m_animationController.m_subWidth,
			m_animationController.m_subHeight,
			&m_color
		);
	}
	else {
		m_material2d->SetMainTexture(m_mainTexture);
		m_material2d->PrepareShader(m_WVP, 1, 1, 0, 0, 1, 1, &m_color);
	}

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

UIComponent::RenderType UIComponent::GetRenderType()
{
	return m_renderType;
}

UIComponent::AlignHorizontal UIComponent::GetAlignHorizontal()
{
	return m_alignH;
}

UIComponent::AlignVertical UIComponent::GetAlignVertical()
{
	return m_alignV;
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