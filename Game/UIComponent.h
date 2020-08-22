#pragma once
#include "Sprite.h"
#include <vector>
class UIComponent : public Sprite {
public:
	enum RenderType{Stretch, Fit, Expand};
	enum AlignVertical{Top, Center, Bottom};
	enum AlignHorizontal{Left, Middle, Right};
protected:
	float m_top, m_bottom, m_left, m_right;
	RenderType m_renderType;
	AlignHorizontal m_alignH;
	AlignVertical m_alignV;

	Matrix m_scaleRenderMatrix, m_translateAlignMatrix;
	void CalculateScaleForRenderType(Camera2D* mainCamera);
	void CalculateTranslateForAlign(Camera2D* mainCamera);
public:
	UIComponent(int id);
	virtual ~UIComponent();
	virtual void Render(Camera2D* mainCamera);

	virtual void SetPosition(Vector3 position);

	virtual void SetBound(float top, float bottom, float left, float right);
	virtual void SetRenderType(RenderType type);
	virtual void SetAlignHorizontal(AlignHorizontal h);
	virtual void SetAlignVertical(AlignVertical v);

	virtual void SetTop(float value);
	virtual float GetTop();
	virtual void SetBottom(float value);
	virtual float GetBottom();
	virtual void SetLeft(float value);
	virtual float GetLeft();
	virtual void SetRight(float value);
	virtual float GetRight();
};