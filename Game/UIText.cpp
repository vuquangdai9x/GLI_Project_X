#include "UIText.h"
#include "ResourcesManager2D.h"
#include "../Framework3D/TrainingFramework/framework3d.h"

UIText::UIText(int id) 
	:UIComponent(id), 
	m_font(NULL), m_text(NULL), m_iTextLength(0), m_aCharacterWVP(NULL), m_materialText(NULL)
{}

UIText::~UIText() {
	if (m_aCharacterWVP != NULL) delete[] m_aCharacterWVP;
	if (m_text != NULL) delete[] m_text;
}

void UIText::Init(Vector3 position, float rotation, Vector2 scale, unsigned int hexColor, float alpha, int iMaterialId, int iMainTexId) {
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);
	SetColor(hexColor, alpha);
	m_model = Singleton<ResourceManager2D>::GetInstance()->GetModel(1);
	if (m_model == NULL) {
		printf("[ERR] UIText: Failed to get model %d\n", 0);
	}
	m_materialText = dynamic_cast<MaterialText2D*>(Singleton<ResourceManager2D>::GetInstance()->GetMaterial(iMaterialId));
	if (m_materialText == NULL) {
		printf("[ERR] UIText: Fail to load material text: %d\n", iMaterialId);
	}
	m_mainTexture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iMainTexId);
	if (m_mainTexture == NULL) {
		printf("[ERR] UIText: Failed to get main texture %d\n", iMainTexId);
	}
	m_originSize.x = m_originSize.y = 0;
}

void UIText::Render(Camera2D* mainCamera) {
	if (m_materialText == NULL || m_model == NULL || m_font == NULL) return;

	CalculateUITransform(mainCamera);
	m_WVP = m_scaleUIMatrix * m_transformMat * m_translateAlignMatrix;

	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	for (int i=0; i<m_iTextLength; i++)
	{
		//m_charWVP = Matrix().SetScale(scaleSize) * m_WVP;
		FontFace::Character ch = m_font->GetCharacter(m_text[i]);
		m_materialText->SetTextureHandle(ch.uiTextureHandle);
		m_materialText->PrepareShader(m_aCharacterWVP[i] * m_WVP, 1, 1, 0, 0, 1, 1, &m_color);
		glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const char * UIText::GetText() { return m_text; }
void UIText::SetText(char* text) {
	if (m_text != NULL) delete[] m_text;
	m_iTextLength = strlen(text);
	m_text = new char[m_iTextLength + 1];
	strcpy(m_text, text);
	if (m_aCharacterWVP != NULL) delete[] m_aCharacterWVP;
	m_aCharacterWVP = new Matrix[m_iTextLength];
	CalculateOriginSize();
}
void UIText::SetFont(int iFontFaceId) {
	m_font = Singleton<ResourceManager2D>::GetInstance()->GetFontFace(iFontFaceId);
	if (m_font == NULL) {
		printf("[ERR] UIText: Fail to load font: %d\n", iFontFaceId);
	}
	CalculateOriginSize();
}
void UIText::CalculateOriginSize() {
	if (m_font == NULL || m_text == NULL) return;
	m_originSize.x = 0;
	
	int* aiOffsetX = new int[m_iTextLength];
	for (int i = 0; i < m_iTextLength; i++)
	{
		aiOffsetX[i] = m_originSize.x;
		m_originSize.x += m_font->GetCharacter(m_text[i]).uiAdvance;
	}
	m_originSize.y = m_font->GetMaxY() - m_font->GetMinY();

	Vector3 chrT, chrS; // character translate, scale
	float chrR; // character rotation
	Matrix mT, mS, mR;
	float baselineTranslateY = (m_font->GetMaxY() + m_font->GetMinY()) / 2.0; // in pixel
	for (int i = 0; i < m_iTextLength; i++)
	{
		FontFace::Character ch = m_font->GetCharacter(m_text[i]);
		chrT.x = (aiOffsetX[i] + ch.iBearingX + ch.iWidth / 2) / m_originSize.x * 2 - 1;
		chrT.y = ((ch.iBearingY - ch.iHeight / 2) - baselineTranslateY) / m_originSize.y * 2;
		chrT.z = 0; // not translate Z position
		chrS.x = ch.iWidth / m_originSize.x;
		chrS.y = ch.iHeight / m_originSize.y;
		chrR = 0;
		m_aCharacterWVP[i] = mS.SetScale(chrS) * mR.SetRotationZ(chrR) * mT.SetTranslation(chrT);
	}

	// originSize still in pixel unit. We have to divide it to ppu
	m_originSize /= Globals::pixelPerUnit;

	SetBound(m_top, m_bottom, m_left, m_right); // re-calculate position and scale because originSize has change

	delete[] aiOffsetX;
}