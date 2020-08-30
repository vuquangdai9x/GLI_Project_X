#include "SpriteLoopTexture.h"

SpriteLoopTexture::SpriteLoopTexture(int id):Sprite(id)
{
	m_originPos = Vector2(0, 0);
	m_iRenderLoopAmount = 1;
}

SpriteLoopTexture::~SpriteLoopTexture()
{
}

void SpriteLoopTexture::Render(Camera2D* mainCamera)
{
	if (m_material2d == NULL || m_model == NULL) return;
	m_WVP = m_transformMat * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	Vector2 offset;

	if (m_isUseAnimation) {
		offset.x = (m_position.x - m_originPos.x) / m_animationController.m_frameWidth / m_scale.x * 2;
		offset.y = (m_position.y - m_originPos.y) / m_animationController.m_frameHeight / m_scale.y * 2;

		m_material2d->SetMainTexture(m_animationController.GetTexture());
		m_material2d->PrepareShader(m_WVP,
			m_animationController.m_frameWidth,
			m_animationController.m_frameHeight,
			offset.x,
			offset.y,
			m_animationController.m_subWidth,
			m_animationController.m_subHeight,
			&m_color
		);
	}
	else {
		offset.x = (m_position.x - m_originPos.x) / m_originSize.x / m_scale.x / 2;
		offset.y = (m_position.y - m_originPos.y) / m_originSize.y / m_scale.y / 2;
		m_material2d->SetMainTexture(m_mainTexture);
		m_material2d->PrepareShader(m_WVP, m_originSize.x, m_originSize.y, offset.x, offset.y, 1, 1, &m_color);
	}

	glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteLoopTexture::Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId)
{
	Sprite::Init(position, rotation, scale, hexColor, alpha, iMaterialId, iMainTexId);
	m_originPos = Vector2(position.x,position.y);
}

void SpriteLoopTexture::Init(Sprite& sprite)
{
	Sprite::Init(sprite);
	m_originPos = Vector2(sprite.GetPosition().x, sprite.GetPosition().y);
}

void SpriteLoopTexture::SetOriginPos(Vector2 newPos)
{
	m_originPos = newPos;
}

Vector2 SpriteLoopTexture::GetOriginPos()
{
	return m_originPos;
}

void SpriteLoopTexture::SetLoopAmount(int value) {
	m_iRenderLoopAmount = value;
}
int SpriteLoopTexture::GetLoopAmount() { return m_iRenderLoopAmount; }
