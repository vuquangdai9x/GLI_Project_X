#pragma once
#include "Sprite.h"
#include <vector>
class UIComponent : public Sprite {
public:
	enum RenderType{Stretch, Fit, Expand, FitHeight, FitWidth};
	enum AlignVertical{Top, Middle, Bottom};
	enum AlignHorizontal{Left, Center, Right};
protected:
	float m_top, m_bottom, m_left, m_right;
	RenderType m_renderType;
	AlignHorizontal m_alignH;
	AlignVertical m_alignV;

	Matrix m_scaleUIMatrix, m_translateAlignMatrix;
	void CalculateUITransform(Camera2D* mainCamera);
public:
	UIComponent(int id);
	virtual ~UIComponent();
	virtual void Render(Camera2D* mainCamera);
	
	virtual void SetPosition(Vector3 position);
	virtual void SetBound(float top, float bottom, float left, float right);
	virtual void SetRenderType(RenderType type);
	virtual void SetAlignHorizontal(AlignHorizontal h);
	virtual void SetAlignVertical(AlignVertical v);


	RenderType GetRenderType();
	AlignHorizontal GetAlignHorizontal();
	AlignVertical GetAlignVertical();

	virtual void SetTop(float value);
	virtual void SetBottom(float value);
	virtual void SetLeft(float value);
	virtual void SetRight(float value);
	float GetTop();
	float GetBottom();
	float GetLeft();
	float GetRight();
};