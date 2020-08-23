#pragma once
#include "UIComponent.h"
#include "FontFace.h"
#include "MaterialText2D.h"

class UIText : public UIComponent {
protected:
	const char* m_text;
	int m_iTextLength;
	FontFace* m_font;
	Matrix* m_aCharacterWVP;
	MaterialText2D* m_materialText;
	void CalculateOriginSize();
public:
	UIText(int id);
	virtual ~UIText();
	virtual void Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId);
	virtual void Render(Camera2D* mainCamera);
	void SetFont(int iFontFaceId);
	const char * GetText();
	void SetText(const char* text);
};