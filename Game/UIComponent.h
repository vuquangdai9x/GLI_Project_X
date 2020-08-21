#pragma once
#include "Sprite.h"
#include <vector>
class UIComponent : public Sprite {
public:
	enum RenderType{Stretch, Fit, Expand};
protected:
	float m_top, m_bottom, m_left, m_right;
	RenderType m_renderType;

	std::vector<UIComponent*> m_listChilds;
public:
	UIComponent(int id);
	virtual ~UIComponent();
	void Render(Camera2D* mainCamera);

	virtual void SetPosition(Vector3 position);
	virtual void SetScale(Vector2 scale);

	virtual void SetBound(float top, float bottom, float left, float right);
	virtual void SetRenderType(RenderType type);

	virtual void SetTop(float value);
	virtual float GetTop();
	virtual void SetBottom(float value);
	virtual float GetBottom();
	virtual void SetLeft(float value);
	virtual float GetLeft();
	virtual void SetRight(float value);
	virtual float GetRight();
};