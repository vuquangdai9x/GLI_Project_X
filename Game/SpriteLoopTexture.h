#pragma once
#include "Sprite.h"
class SpriteLoopTexture : public Sprite {
protected:
	Vector2 m_originPos;
	int m_iLoopHorizontalAmount, m_iLoopVerticalAmount;
	Matrix m_loopScaleMatrix;
public:
	SpriteLoopTexture(int id);
	virtual ~SpriteLoopTexture();
	virtual void Render(Camera2D* mainCamera);
	virtual void Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId);
	virtual void Init(Sprite& sprite);
	void SetOriginPos(Vector2 newPos);
	Vector2 GetOriginPos();
	void SetLoopAmount(int iHorizontalAmount, int iVerticalAmount);
};