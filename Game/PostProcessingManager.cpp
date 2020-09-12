#include "PostProcessingManager.h"
#include "../Framework3D/TrainingFramework/framework3d.h"

#include <math.h>

Vector3 hsv2rgb(Vector3 HSV)
{
	Vector3 RGB;
	double H = HSV.x, S = HSV.y, V = HSV.z,
		P, Q, T,
		fract;
	(H == 360.) ? (H = 0.) : (H /= 60.);
	fract = H - floor(H);
	P = V*(1. - S);
	Q = V*(1. - S*fract);
	T = V*(1. - S*(1. - fract));

	if (0. <= H && H < 1.) RGB = Vector3(V, T, P);
	else if (1. <= H && H < 2.) RGB = Vector3(Q, V, P);
	else if (2. <= H && H < 3.) RGB = Vector3(P, V, T);
	else if (3. <= H && H < 4.) RGB = Vector3(P, Q, V);
	else if (4. <= H && H < 5.) RGB = Vector3(T, P, V);
	else if (5. <= H && H < 6.) RGB = Vector3(V, P, Q);
	else RGB = Vector3(0, 0, 0);

	return RGB;
}

Shaders * PostProcessingManager::GetShader(int iShaderId)
{
	for (int i = 0; i < m_listShader.size(); i++) {
		if (m_listShader[i]->m_id == iShaderId) {
			return m_listShader[i];
		}
	}
	return NULL;
}

MaterialPostProcess * PostProcessingManager::GetMaterial(int iMaterialId)
{
	for (int i = 0; i < m_listMaterial.size(); i++) {
		if (m_listMaterial[i]->GetId() == iMaterialId) {
			return m_listMaterial[i];
		}
	}
	return NULL;
}

void PostProcessingManager::ResetColorCorrection()
{
	if (m_materialColorCorrection != NULL) {
		m_materialColorCorrection->m_uLift = Vector3(0, 0, 0);
		m_materialColorCorrection->m_uGamma = Vector3(0, 0, 0);
		m_materialColorCorrection->m_uGain = Vector3(0, 0, 0);
		m_materialColorCorrection->m_uOffset = Vector3(0, 0, 0);
		m_materialColorCorrection->m_uContrast = 1.0;
		m_materialColorCorrection->m_uContrastPivot = 0.5;
		m_materialColorCorrection->m_uSaturation = 1.0;
		m_materialColorCorrection->m_uHue = 0.0;
		m_materialColorCorrection->m_uTemperature = 66.0;
		m_materialColorCorrection->m_uUnsharpMask = 0.0;
	}
	if (m_materialVignette != NULL) {
		m_materialVignette->m_uColor = Vector4(0, 0, 0, 1);
		m_materialVignette->m_uExtend = 0;
		m_materialVignette->m_uIntensify = 0;
	}

	Vector3 handlePos;

	m_lift_UIColorTarget->SetPosition(m_lift_UIColorWheel->GetPosition());
	m_lift_UIHandle->SetPosition(m_lift_UIBar->GetPosition());

	m_gamma_UIColorTarget->SetPosition(m_gamma_UIColorWheel->GetPosition());
	m_gamma_UIHandle->SetPosition(m_gamma_UIBar->GetPosition());

	m_gain_UIColorTarget->SetPosition(m_gain_UIColorWheel->GetPosition());
	m_gain_UIHandle->SetPosition(m_gain_UIBar->GetPosition());

	m_offset_UIColorTarget->SetPosition(m_offset_UIColorWheel->GetPosition());
	m_offset_UIHandle->SetPosition(m_offset_UIBar->GetPosition());

	handlePos = m_unsharpMask_UIHandle->GetPosition();
	handlePos.x = m_unsharpMask_UIBar->GetLeft();
	m_unsharpMask_UIHandle->SetPosition(handlePos);

	m_temperature_UIHandle->SetPosition(m_temperature_UIBar->GetPosition());
	m_hue_UIHandle->SetPosition(m_hue_UIBar->GetPosition());
	m_saturation_UIHandle->SetPosition(m_saturation_UIBar->GetPosition());
	m_contrastPivot_UIHandle->SetPosition(m_contrastPivot_UIBar->GetPosition());
	m_contrast_UIHandle->SetPosition(m_contrast_UIBar->GetPosition());

	handlePos = m_vignetteIntensify_UIHandle->GetPosition();
	handlePos.x = m_vignetteIntensify_UIBar->GetLeft();
	m_vignetteIntensify_UIHandle->SetPosition(handlePos);

	handlePos = m_vignetteExtend_UIHandle->GetPosition();
	handlePos.x = m_vignetteExtend_UIBar->GetLeft();
	m_vignetteExtend_UIHandle->SetPosition(handlePos);

	handlePos = m_vignetteBrightness_UIHandle->GetPosition();
	handlePos.x = m_vignetteBrightness_UIBar->GetLeft();
	m_vignetteBrightness_UIHandle->SetPosition(handlePos);

	handlePos = m_vignetteOpacity_UIHandle->GetPosition();
	handlePos.x = m_vignetteOpacity_UIBar->GetRight();
	m_vignetteOpacity_UIHandle->SetPosition(handlePos);
}

PostProcessingManager::PostProcessingManager()
{
	// color correction
	m_isEnabledColorCorrection = true;
	m_materialColorCorrection = NULL;
	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &m_frameBufObjColorCorrection);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufObjColorCorrection);
	// create a color attachment texture
	glGenTextures(1, &m_textureHandleColorCorrection);
	glBindTexture(GL_TEXTURE_2D, m_textureHandleColorCorrection);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::screenWidth, Globals::screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureHandleColorCorrection, 0);
	// create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &m_depthBufObjColorCorrection);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufObjColorCorrection);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Globals::screenWidth, Globals::screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufObjColorCorrection); // now actually attach it
																									// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		m_canRunColorCorrection = false;
		printf("[ERR] PostProcessingManager: Color Correction Framebuffer is not complete!\n");
	}

	// user define
	m_isEnabled = m_canRun = true;
	m_materialColorCorrection = NULL;
	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &m_frameBufObj);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufObj);
	// create a color attachment texture
	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::screenWidth, Globals::screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureHandle, 0);
	// create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &m_depthBufObj);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufObj);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Globals::screenWidth, Globals::screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufObj); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		m_canRun = false;
		printf("[ERR] PostProcessingManager: Framebuffer is not complete!\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CreateUI();
}


void PostProcessingManager::CreateUI()
{
	
	m_turnOnColorCorrectionDebug = true;
	m_isUseUI = false;

	m_colorBlendUI_top = -0.5;
	m_colorBlendUI_bottom = -1;
	m_colorBlendUI_left = -1;
	m_colorBlendUI_right = 1;

	m_colorCorrectionUI_top = 0.5;
	m_colorCorrectionUI_bottom = -0.5;
	m_colorCorrectionUI_left = -1;
	m_colorCorrectionUI_right = 0;

	m_vignetteUI_top = 1;
	m_vignetteUI_bottom = 0.5;
	m_vignetteUI_left = -1;
	m_vignetteUI_right = 0;

	float colorBlendUI_height = m_colorBlendUI_top - m_colorBlendUI_bottom;
	float colorBlendUI_width = m_colorBlendUI_right - m_colorBlendUI_left;
	float colorCorrectionUI_height = m_colorCorrectionUI_top - m_colorCorrectionUI_bottom;
	float colorCorrectionUI_width = m_colorCorrectionUI_right - m_colorCorrectionUI_left;
	float vignetteUI_height = m_vignetteUI_top - m_vignetteUI_bottom;
	float vignetteUI_width = m_vignetteUI_right - m_vignetteUI_left;

	m_camera = new Camera2D();
	m_camera->Init(Vector3(0,0,0), 0);
	float aspectRatio = Globals::screenWidth / (float)Globals::screenHeight;
	m_camera->SetOrthorgraphic(40, aspectRatio, 0.1, 100.0);

	UIText* text;
	Vector2 barScale(0.9,0.9);

	// lift
	text = new UIText(10);
	text->Init(Vector3(0,0,-1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Lift");
	text->SetBound(
		m_colorBlendUI_top,
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_left,
		m_colorBlendUI_left + colorBlendUI_width * 0.25
	);
	m_listText.push_back(text);

	m_lift_UIColorWheel = new UIComponent(11);
	m_lift_UIColorWheel->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 60601);
	m_lift_UIColorWheel->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_left,
		m_colorBlendUI_left + colorBlendUI_width * 0.25
	);

	m_lift_UIColorTarget = new UIComponent(12);
	m_lift_UIColorTarget->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 55201);
	m_lift_UIColorTarget->SetBound(0.05, 0, 0, 0.05);
	

	m_lift_UIBar = new UIComponent(13);
	m_lift_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_lift_UIBar->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height*1,
		m_colorBlendUI_left,
		m_colorBlendUI_left + colorBlendUI_width * 0.25
	);
	m_lift_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_lift_UIHandle = new UIComponent(14);
	m_lift_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_lift_UIHandle->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		0, 0.05
	);
	m_lift_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	

	// gamma
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Gamma");
	text->SetBound(
		m_colorBlendUI_top,
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_left + colorBlendUI_width * 0.25,
		m_colorBlendUI_left + colorBlendUI_width * 0.5
	);
	m_listText.push_back(text);

	m_gamma_UIColorWheel = new UIComponent(11);
	m_gamma_UIColorWheel->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 60601);
	m_gamma_UIColorWheel->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_left + colorBlendUI_width * 0.25,
		m_colorBlendUI_left + colorBlendUI_width * 0.5
	);

	m_gamma_UIColorTarget = new UIComponent(12);
	m_gamma_UIColorTarget->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 55201);
	m_gamma_UIColorTarget->SetBound(0.05, 0, 0, 0.05);
	

	m_gamma_UIBar = new UIComponent(13);
	m_gamma_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_gamma_UIBar->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		m_colorBlendUI_left + colorBlendUI_width * 0.25,
		m_colorBlendUI_left + colorBlendUI_width * 0.5
	);
	m_gamma_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_gamma_UIHandle = new UIComponent(14);
	m_gamma_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_gamma_UIHandle->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		0, 0.05
	);
	m_gamma_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	

	// gain
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Gain");
	text->SetBound(
		m_colorBlendUI_top,
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_left + colorBlendUI_width * 0.5,
		m_colorBlendUI_left + colorBlendUI_width * 0.75
	);
	m_listText.push_back(text);

	m_gain_UIColorWheel = new UIComponent(11);
	m_gain_UIColorWheel->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 60601);
	m_gain_UIColorWheel->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_left + colorBlendUI_width * 0.5,
		m_colorBlendUI_left + colorBlendUI_width * 0.75
	);

	m_gain_UIColorTarget = new UIComponent(12);
	m_gain_UIColorTarget->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 55201);
	m_gain_UIColorTarget->SetBound(0.05, 0, 0, 0.05);
	

	m_gain_UIBar = new UIComponent(13);
	m_gain_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_gain_UIBar->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		m_colorBlendUI_left + colorBlendUI_width * 0.5,
		m_colorBlendUI_left + colorBlendUI_width * 0.75
	);
	m_gain_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_gain_UIHandle = new UIComponent(14);
	m_gain_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_gain_UIHandle->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		0, 0.05
	);
	m_gain_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// offset
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Offset");
	text->SetBound(
		m_colorBlendUI_top,
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_left + colorBlendUI_width * 0.75,
		m_colorBlendUI_left + colorBlendUI_width * 1
	);
	m_listText.push_back(text);

	m_offset_UIColorWheel = new UIComponent(11);
	m_offset_UIColorWheel->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 60601);
	m_offset_UIColorWheel->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.2,
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_left + colorBlendUI_width * 0.75,
		m_colorBlendUI_left + colorBlendUI_width * 1
	);

	m_offset_UIColorTarget = new UIComponent(12);
	m_offset_UIColorTarget->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 0, 55201);
	m_offset_UIColorTarget->SetBound(0.05, 0, 0, 0.05);
	
	m_offset_UIBar = new UIComponent(13);
	m_offset_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_offset_UIBar->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		m_colorBlendUI_left + colorBlendUI_width * 0.75,
		m_colorBlendUI_left + colorBlendUI_width * 1
	);
	m_offset_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_offset_UIHandle = new UIComponent(14);
	m_offset_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_offset_UIHandle->SetBound(
		m_colorBlendUI_top - colorBlendUI_height*0.8,
		m_colorBlendUI_top - colorBlendUI_height * 1,
		0, 0.05
	);
	m_offset_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// contrast
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Contrast");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 0.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0/6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_contrast_UIBar = new UIComponent(13);
	m_contrast_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_contrast_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 0.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_contrast_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_contrast_UIHandle = new UIComponent(14);
	m_contrast_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_contrast_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 0.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0 / 6.0,
		0, 0.05
	);
	m_contrast_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// contrast pivot
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("CPivot");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2.0 / 6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_contrastPivot_UIBar = new UIComponent(13);
	m_contrastPivot_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_contrastPivot_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2.0 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_contrastPivot_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_contrastPivot_UIHandle = new UIComponent(14);
	m_contrastPivot_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_contrastPivot_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 1.0 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2.0 / 6.0,
		0, 0.05
	);
	m_contrastPivot_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// saturation
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Sat");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 3 / 6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_saturation_UIBar = new UIComponent(13);
	m_saturation_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_saturation_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 3 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_saturation_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_saturation_UIHandle = new UIComponent(14);
	m_saturation_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_saturation_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height* 2 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height* 3 / 6.0,
		0, 0.05
	);
	m_saturation_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// hue
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Hue");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 3 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_hue_UIBar = new UIComponent(13);
	m_hue_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_hue_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 3 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_hue_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_hue_UIHandle = new UIComponent(14);
	m_hue_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_hue_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 3 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		0, 0.05
	);
	m_hue_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// tempaerature
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Temp");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_temperature_UIBar = new UIComponent(13);
	m_temperature_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_temperature_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_temperature_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_temperature_UIHandle = new UIComponent(14);
	m_temperature_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_temperature_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 4 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		0, 0.05
	);
	m_temperature_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);
	
	// unsharp mask
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("Unsharp");
	text->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 6 / 6.0,
		m_colorCorrectionUI_left,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2
	);
	m_listText.push_back(text);

	m_unsharpMask_UIBar = new UIComponent(13);
	m_unsharpMask_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_unsharpMask_UIBar->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 6 / 6.0,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 0.2,
		m_colorCorrectionUI_left + colorCorrectionUI_width * 1
	);
	m_unsharpMask_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_unsharpMask_UIHandle = new UIComponent(14);
	m_unsharpMask_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1,1), 0x000000, 0.5, 0, 60102);
	m_unsharpMask_UIHandle->SetBound(
		m_colorCorrectionUI_top - colorCorrectionUI_height * 5 / 6.0,
		m_colorCorrectionUI_top - colorCorrectionUI_height * 6 / 6.0,
		0, 0.05
	);
	m_unsharpMask_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);

	// vignette intensity
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("VigIntensity");
	text->SetBound(
		m_vignetteUI_top - vignetteUI_height * 0 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		m_vignetteUI_left,
		m_vignetteUI_left + vignetteUI_width * 0.2
	);
	m_listText.push_back(text);

	m_vignetteIntensify_UIBar = new UIComponent(13);
	m_vignetteIntensify_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_vignetteIntensify_UIBar->SetBound(
		m_vignetteUI_top - vignetteUI_height * 0 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		m_vignetteUI_left + vignetteUI_width * 0.2,
		m_vignetteUI_left + vignetteUI_width * 1
	);
	m_vignetteIntensify_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_vignetteIntensify_UIHandle = new UIComponent(14);
	m_vignetteIntensify_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0x000000, 0.5, 0, 60102);
	m_vignetteIntensify_UIHandle->SetBound(
		m_vignetteUI_top - vignetteUI_height * 0 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		0, 0.05
	);
	m_vignetteIntensify_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);

	// vignette extend
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("VigExtend");
	text->SetBound(
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		m_vignetteUI_left,
		m_vignetteUI_left + vignetteUI_width * 0.2
	);
	m_listText.push_back(text);

	m_vignetteExtend_UIBar = new UIComponent(13);
	m_vignetteExtend_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_vignetteExtend_UIBar->SetBound(
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		m_vignetteUI_left + vignetteUI_width * 0.2,
		m_vignetteUI_left + vignetteUI_width * 1
	);
	m_vignetteExtend_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_vignetteExtend_UIHandle = new UIComponent(14);
	m_vignetteExtend_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0x000000, 0.5, 0, 60102);
	m_vignetteExtend_UIHandle->SetBound(
		m_vignetteUI_top - vignetteUI_height * 1 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		0, 0.05
	);
	m_vignetteExtend_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);

	// vignette brightness
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("VigBright");
	text->SetBound(
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		m_vignetteUI_left,
		m_vignetteUI_left + vignetteUI_width * 0.2
	);
	m_listText.push_back(text);

	m_vignetteBrightness_UIBar = new UIComponent(13);
	m_vignetteBrightness_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_vignetteBrightness_UIBar->SetBound(
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		m_vignetteUI_left + vignetteUI_width * 0.2,
		m_vignetteUI_left + vignetteUI_width * 1
	);
	m_vignetteBrightness_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_vignetteBrightness_UIHandle = new UIComponent(14);
	m_vignetteBrightness_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0x000000, 0.5, 0, 60102);
	m_vignetteBrightness_UIHandle->SetBound(
		m_vignetteUI_top - vignetteUI_height * 2 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		0, 0.05
	);
	m_vignetteBrightness_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);

	// vignette opacity
	text = new UIText(10);
	text->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0xFFFFFF, 0.5, 1, 0);
	text->SetFont(2);
	text->SetText("VigAlpha");
	text->SetBound(
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 4 / 4.0,
		m_vignetteUI_left,
		m_vignetteUI_left + vignetteUI_width * 0.2
	);
	m_listText.push_back(text);

	m_vignetteOpacity_UIBar = new UIComponent(13);
	m_vignetteOpacity_UIBar->Init(Vector3(0, 0, -1), 0, barScale, 0xFFFFFF, 0.5, 0, 60102);
	m_vignetteOpacity_UIBar->SetBound(
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 4 / 4.0,
		m_vignetteUI_left + vignetteUI_width * 0.2,
		m_vignetteUI_left + vignetteUI_width * 1
	);
	m_vignetteOpacity_UIBar->SetRenderType(UIComponent::RenderType::Stretch);

	m_vignetteOpacity_UIHandle = new UIComponent(14);
	m_vignetteOpacity_UIHandle->Init(Vector3(0, 0, -1), 0, Vector2(1, 1), 0x000000, 0.5, 0, 60102);
	m_vignetteOpacity_UIHandle->SetBound(
		m_vignetteUI_top - vignetteUI_height * 3 / 4.0,
		m_vignetteUI_top - vignetteUI_height * 4 / 4.0,
		0, 0.05
	);
	m_vignetteOpacity_UIHandle->SetRenderType(UIComponent::RenderType::Stretch);

	ResetColorCorrection();
}

PostProcessingManager::~PostProcessingManager()
{
	for (int i = 0; i < m_listMaterial.size(); i++) {
		delete m_listMaterial[i];
	}
	for (int i = 0; i < m_listShader.size(); i++) {
		delete m_listShader[i];
	}

	delete m_lift_UIColorWheel, delete m_lift_UIColorTarget, delete m_lift_UIBar, delete m_lift_UIHandle;
	delete m_gamma_UIColorWheel, delete m_gamma_UIColorTarget, delete m_gamma_UIBar, delete m_gamma_UIHandle;
	delete m_gain_UIColorWheel, delete m_gain_UIColorTarget, delete m_gain_UIBar, delete m_gain_UIHandle;
	delete m_offset_UIColorWheel, delete m_offset_UIColorTarget, delete m_offset_UIBar, delete m_offset_UIHandle;
	delete m_contrast_UIBar, delete m_contrast_UIHandle;
	delete m_contrastPivot_UIBar, delete m_contrastPivot_UIHandle;
	delete m_saturation_UIBar, delete m_saturation_UIHandle;
	delete m_hue_UIBar, delete m_hue_UIHandle;
	delete m_temperature_UIBar, delete m_temperature_UIHandle;
	delete m_unsharpMask_UIBar, delete m_unsharpMask_UIHandle;
	delete m_vignetteIntensify_UIBar, delete m_vignetteIntensify_UIHandle;
	delete m_vignetteExtend_UIBar, delete m_vignetteExtend_UIHandle;
	delete m_vignetteBrightness_UIBar, delete m_vignetteBrightness_UIHandle;
	delete m_vignetteOpacity_UIBar, delete m_vignetteOpacity_UIHandle;
	for (int i = 0; i < m_listText.size(); i++) {
		delete m_listText[i];
	}
	m_listText.clear();
	delete m_camera;
}

void PostProcessingManager::Load(char * postProcessFilePath)
{
	const char* resourceDir = Globals::resourceDir;

	// NOTE: length of file path can be not enough and cause error
	char fileName[256];
	char filePath[512];

	strcpy(filePath, resourceDir);
	strcat(filePath, postProcessFilePath);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("[ERR] PostProcessingManager::Load: Fails to load post process file\n");
		return;
	}

	int iModelId;
	fscanf(fIn, "#Model %d\n", &iModelId);
	if (m_model = Singleton<ResourceManager2D>::GetInstance()->GetModel(iModelId)) {
		printf("[msg] PostProcessingManager::Load: Loaded model %d\n", iModelId);
	}
	else {
		printf("[ERR] PostProcessingManager::Load: Fails to load model %d\n", iModelId);
		m_canRun = false;
	}

	//
	// Load Shaders
	//

	int iNumOfShaders;
	// NOTE: length of file name can be not enough and cause error
	char vertexShaderFile[256], fragmentShaderFile[256];
	fscanf(fIn, "\n#Shaders: %d\n", &iNumOfShaders);
	for (int i = 0; i < iNumOfShaders; i++) {
		Shaders* shader;
		char shaderType[20];
		int iShaderId, iNumOfState;
		char stateSetting[20];

		fscanf(fIn, "ID %d\n", &iShaderId);

		fscanf(fIn, "VS: \"%[^\"]\"\n", fileName);
		strcpy(vertexShaderFile, resourceDir);
		strcat(vertexShaderFile, fileName);
		fscanf(fIn, "FS: \"%[^\"]\"\n", fileName);
		strcpy(fragmentShaderFile, resourceDir);
		strcat(fragmentShaderFile, fileName);

		fscanf(fIn, "STATES %d\n", &iNumOfState);
		for (int j = 0; j < iNumOfState; j++) {
			fscanf(fIn, "STATE %s\n", stateSetting);
		}
		shader = new Shaders(iShaderId);
		if (shader->Init(vertexShaderFile, fragmentShaderFile)) {
			m_listShader.push_back(shader);
			printf("[msg] PostProcessingManager::Init: Loaded Shader %d :\n\tVS: %s\n\tFS: %s\n", iShaderId, vertexShaderFile, fragmentShaderFile);
		}
		else {
			printf("[ERR] PostProcessingManager::Init: Failed to load Shader \n\tVS: %s\n\tFS: %s\n", vertexShaderFile, fragmentShaderFile);
			delete shader;
		}
	}

	// set up vignette
	int iVignetteShaderId;
	fscanf(fIn, "\n#Vignette\n");
	fscanf(fIn, "SHADER %d\n", &iVignetteShaderId);
	m_materialVignette = new MaterialVignette(0);
	if (m_materialVignette->Init(GetShader(iVignetteShaderId))) {
		printf("[msg] PostProcessingManager::Load: Loaded Vignette Material: Shader = %d\n", iVignetteShaderId);
	}
	else {
		printf("[ERR] PostProcessingManager::Load: Failed to load Vignette Material: Shader = %d\n", iVignetteShaderId);
		delete m_materialVignette;
		m_materialVignette = NULL;
	}

	// set up color correction
	int iColorCorrectionShaderId;
	fscanf(fIn, "\n#ColorCorrection\n");
	fscanf(fIn, "SHADER %d\n", &iColorCorrectionShaderId);
	m_materialColorCorrection = new MaterialPPColorCorrection(0);
	if (m_materialColorCorrection->Init(GetShader(iColorCorrectionShaderId))) {
		printf("[msg] PostProcessingManager::Load: Loaded Color Correction Material: Shader = %d\n", iColorCorrectionShaderId);
	}
	else {
		printf("[ERR] PostProcessingManager::Load: Failed to load Color Correction Material: Shader = %d\n", iColorCorrectionShaderId);
		delete m_materialColorCorrection;
		m_materialColorCorrection = NULL;
	}

	//
	// Load Materials
	//

	int iNumOfMaterial;
	fscanf(fIn, "\n#Materials: %d\n", &iNumOfMaterial);
	for (int i = 0; i < iNumOfMaterial; i++) {
		int iMaterialId;
		char materialType[20];
		fscanf(fIn, "ID %d %s\n", &iMaterialId, materialType);
		if (strcmp("DEFAULT", materialType) == 0) {
			MaterialPostProcess* pMaterial;
			int iShaderId;
			pMaterial = new MaterialPostProcess(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			if (pMaterial->Init(GetShader(iShaderId))) {
				m_listMaterial.push_back(pMaterial);
				printf("[msg] PostProcessingManager::Load: Loaded Particles Material %d : Shader = %d\n", iMaterialId, iShaderId);
			}
			else {
				printf("[ERR] PostProcessingManager::Load: Failed to load Particles Material %d : Shader = %d\n", iMaterialId, iShaderId);
				delete pMaterial;
			}
		}
		else {
			printf("[ERR] PostProcessingManager::Load: Material type invalid\n");
		}
	}

	fclose(fIn);
}

void PostProcessingManager::SetMaterial(int iMaterialId)
{
	if (m_material = GetMaterial(iMaterialId)) {
		printf("[ERR] PostProcessingManager::SetMaterial: Set Material %d\n", iMaterialId);
	}
	else {
		m_canRun = false;
		printf("[ERR] PostProcessingManager::SetMaterial: Material %d not found\n", iMaterialId);
	}
}

void PostProcessingManager::RenderVignette() {
	if (m_materialVignette->m_uIntensify <= 0 || m_materialVignette->m_uExtend <= 0) return;
	glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);

	m_materialVignette->PrepareShader(m_textureHandle);
	glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PostProcessingManager::Prepare()
{
	if (m_isEnabled && m_canRun) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufObjColorCorrection);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void PostProcessingManager::Render()
{
	if (m_isEnabled && m_canRun) {
		glBindBuffer(GL_ARRAY_BUFFER, m_model->m_vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->m_iboId);
		
		// color correction
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufObj);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_materialColorCorrection->PrepareShader(m_textureHandleColorCorrection);
		glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

		// render to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_material->PrepareShader(m_textureHandle);
		glDrawElements(GL_TRIANGLES, m_model->m_iNumOfIndice, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (m_isUseUI && m_isEnabledColorCorrection) {
		m_lift_UIColorWheel->Render(m_camera);
		m_lift_UIColorTarget->Render(m_camera);
		m_lift_UIBar->Render(m_camera);
		m_lift_UIHandle->Render(m_camera);
		m_gamma_UIColorWheel->Render(m_camera);
		m_gamma_UIColorTarget->Render(m_camera);
		m_gamma_UIBar->Render(m_camera);
		m_gamma_UIHandle->Render(m_camera);
		m_gain_UIColorWheel->Render(m_camera);
		m_gain_UIColorTarget->Render(m_camera);
		m_gain_UIBar->Render(m_camera);
		m_gain_UIHandle->Render(m_camera);
		m_offset_UIColorWheel->Render(m_camera);
		m_offset_UIColorTarget->Render(m_camera);
		m_offset_UIBar->Render(m_camera);
		m_offset_UIHandle->Render(m_camera);
		m_contrast_UIBar->Render(m_camera);
		m_contrast_UIHandle->Render(m_camera);
		m_contrastPivot_UIBar->Render(m_camera);
		m_contrastPivot_UIHandle->Render(m_camera);
		m_saturation_UIBar->Render(m_camera);
		m_saturation_UIHandle->Render(m_camera);
		m_hue_UIBar->Render(m_camera);
		m_hue_UIHandle->Render(m_camera);
		m_temperature_UIBar->Render(m_camera);
		m_temperature_UIHandle->Render(m_camera);
		m_unsharpMask_UIBar->Render(m_camera);
		m_unsharpMask_UIHandle->Render(m_camera);
		m_vignetteIntensify_UIBar->Render(m_camera);
		m_vignetteIntensify_UIHandle->Render(m_camera);
		m_vignetteExtend_UIBar->Render(m_camera);
		m_vignetteExtend_UIHandle->Render(m_camera);
		m_vignetteBrightness_UIBar->Render(m_camera);
		m_vignetteBrightness_UIHandle->Render(m_camera);
		m_vignetteOpacity_UIBar->Render(m_camera);
		m_vignetteOpacity_UIHandle->Render(m_camera);
		for (int i = 0; i < m_listText.size(); i++) {
			m_listText[i]->Render(m_camera);
		}
	}
}

void PostProcessingManager::Update(float deltaTime)
{
	Singleton<InputManager>::GetInstance()->m_isLocked = false;

	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::O)) {
		ResetColorCorrection();
	}
	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::P)) {
		if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT))
			SetShowUI(false);
		else
			SetShowUI(true);
	}
	
	if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_CLICK) {
		m_isPressingToggle = true;
	}
	else if (Singleton<InputManager>::GetInstance()->getMouseEvent() == MOUSE_RELEASE) {
		m_isPressingToggle = false;
	}
	
	if (m_isUseUI) {
		if (m_isPressingToggle) {
			if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT)) {
				if (m_lift_UIColorWheel->CheckMouseInside() || m_lift_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uLift = Vector3(0, 0, 0);
					m_lift_UIColorTarget->SetPosition(m_lift_UIColorWheel->GetPosition());
					m_lift_UIHandle->SetPosition(m_lift_UIBar->GetPosition());
				}
				else if (m_gamma_UIColorWheel->CheckMouseInside() || m_gamma_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uGamma = Vector3(0, 0, 0);
					m_gamma_UIColorTarget->SetPosition(m_gamma_UIColorWheel->GetPosition());
					m_gamma_UIHandle->SetPosition(m_gamma_UIBar->GetPosition());
				}
				if (m_gain_UIColorWheel->CheckMouseInside() || m_gain_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uGain = Vector3(0, 0, 0);
					m_gain_UIColorTarget->SetPosition(m_gain_UIColorWheel->GetPosition());
					m_gain_UIHandle->SetPosition(m_gain_UIBar->GetPosition());
				}
				else if (m_offset_UIColorWheel->CheckMouseInside() || m_offset_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uOffset = Vector3(0, 0, 0);
					m_offset_UIColorTarget->SetPosition(m_offset_UIColorWheel->GetPosition());
					m_offset_UIHandle->SetPosition(m_offset_UIBar->GetPosition());
				}
				else if (m_contrast_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uContrast = 1.0;
					m_contrast_UIHandle->SetPosition(m_contrast_UIBar->GetPosition());
				}
				else if (m_contrastPivot_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uContrastPivot = 0.5;
					m_contrastPivot_UIHandle->SetPosition(m_contrastPivot_UIBar->GetPosition());
				}
				else if (m_saturation_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uSaturation = 1.0;
					m_saturation_UIHandle->SetPosition(m_saturation_UIBar->GetPosition());
				}
				else if (m_hue_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uHue = 0.0;
					m_hue_UIHandle->SetPosition(m_hue_UIBar->GetPosition());
				}
				else if (m_temperature_UIBar->CheckMouseInside()) {
					m_materialColorCorrection->m_uTemperature = 66.0;
					m_temperature_UIHandle->SetPosition(m_temperature_UIBar->GetPosition());
				}
				else if (m_unsharpMask_UIBar->CheckMouseInside()) {
					Vector3 handlePos;
					m_materialColorCorrection->m_uUnsharpMask = 0.0;
					handlePos = m_unsharpMask_UIHandle->GetPosition();
					handlePos.x = m_unsharpMask_UIBar->GetLeft();
					m_unsharpMask_UIHandle->SetPosition(handlePos);
				}
				else if (m_vignetteExtend_UIBar->CheckMouseInside()) {
					m_materialVignette->m_uExtend = 0;
					Vector3 handlePos;
					handlePos = m_vignetteExtend_UIHandle->GetPosition();
					handlePos.x = m_vignetteExtend_UIBar->GetLeft();
					m_vignetteExtend_UIHandle->SetPosition(handlePos);
				}
				else if (m_vignetteIntensify_UIBar->CheckMouseInside()) {
					m_materialVignette->m_uIntensify = 0;
					Vector3 handlePos;
					handlePos = m_vignetteIntensify_UIHandle->GetPosition();
					handlePos.x = m_vignetteIntensify_UIBar->GetLeft();
					m_vignetteIntensify_UIHandle->SetPosition(handlePos);
				}
				else if (m_vignetteBrightness_UIBar->CheckMouseInside()) {
					m_materialVignette->m_uColor = Vector4(0, 0, 0, m_materialVignette->m_uColor.w);
					Vector3 handlePos;
					handlePos = m_vignetteBrightness_UIHandle->GetPosition();
					handlePos.x = m_vignetteBrightness_UIBar->GetLeft();
					m_vignetteBrightness_UIHandle->SetPosition(handlePos);
				}
				else if (m_vignetteOpacity_UIBar->CheckMouseInside()) {
					m_materialVignette->m_uColor.w = 1;
					Vector3 handlePos;
					handlePos = m_vignetteOpacity_UIHandle->GetPosition();
					handlePos.x = m_vignetteOpacity_UIBar->GetRight();
					m_vignetteOpacity_UIHandle->SetPosition(handlePos);
				}
				m_isPressingToggle = false;
			}
			else {
				int iMousePosX, iMousePosY;
				Singleton<InputManager>::GetInstance()->getXY(iMousePosX, iMousePosY);
				// chuyen ve toa do [-1..1]
				float mousePosX = iMousePosX / (float)Globals::screenWidth * 2 - 1;
				float mousePosY = iMousePosY / (float)Globals::screenHeight * 2 - 1;
				mousePosY *= -1; // toa do chuot nguoc voi toa do UI
				Vector3 hsv;
				Vector2 deltaPos;
				float colorWheelRadius;
				float barLength;
				bool isChanged;

				m_isPressingToggle = false;

				// lift
				colorWheelRadius = min(m_lift_UIColorWheel->GetTop() - m_lift_UIColorWheel->GetBottom(), m_lift_UIColorWheel->GetRight() - m_lift_UIColorWheel->GetLeft()) / 2.0;
				barLength = (m_lift_UIBar->GetRight() - m_lift_UIBar->GetLeft()) * m_lift_UIBar->GetScale().x;
				isChanged = false;
				if (m_lift_UIColorWheel->CheckMouseInside()) {
					isChanged = true;
					m_lift_UIColorTarget->SetPosition(Vector3(mousePosX, mousePosY, m_lift_UIColorTarget->GetPosition().z));
				}
				if (m_lift_UIBar->CheckMouseInside()) {
					isChanged = true;
					m_lift_UIHandle->SetPosition(Vector3(mousePosX, m_lift_UIBar->GetPosition().y, m_lift_UIBar->GetPosition().z));
				}
				if (isChanged) {
					m_isPressingToggle = true;
					deltaPos.x = m_lift_UIColorTarget->GetPosition().x - m_lift_UIColorWheel->GetPosition().x;
					deltaPos.y = m_lift_UIColorTarget->GetPosition().y - m_lift_UIColorWheel->GetPosition().y;
					hsv.y = deltaPos.Length() / colorWheelRadius;
					hsv.z = (m_lift_UIHandle->GetPosition().x - m_lift_UIBar->GetLeft()) / barLength;

					hsv.z = hsv.z * 2 - 1;
					deltaPos = deltaPos.Normalize();
					hsv.x = acosf(deltaPos.x) * (deltaPos.y > 0 ? 1 : -1) * 180.0 / M_PI;
					hsv.x = hsv.x * -1 + 90;
					if (hsv.z < 0) hsv.x += 180;
					if (hsv.x < 0.0) hsv.x += 360.0;
					if (hsv.x > 360.0) hsv.x -= 360.0;

					m_materialColorCorrection->m_uLift = hsv2rgb(hsv);
				}

				// gamma
				isChanged = false;
				if (m_gamma_UIColorWheel->CheckMouseInside()) {
					isChanged = true;
					m_gamma_UIColorTarget->SetPosition(Vector3(mousePosX, mousePosY, m_gamma_UIColorTarget->GetPosition().z));
				}
				if (m_gamma_UIBar->CheckMouseInside()) {
					isChanged = true;
					m_gamma_UIHandle->SetPosition(Vector3(mousePosX, m_gamma_UIBar->GetPosition().y, m_gamma_UIBar->GetPosition().z));
				}
				if (isChanged) {
					m_isPressingToggle = true;
					deltaPos.x = m_gamma_UIColorTarget->GetPosition().x - m_gamma_UIColorWheel->GetPosition().x;
					deltaPos.y = m_gamma_UIColorTarget->GetPosition().y - m_gamma_UIColorWheel->GetPosition().y;
					hsv.y = deltaPos.Length() / colorWheelRadius;
					hsv.z = (m_gamma_UIHandle->GetPosition().x - m_gamma_UIBar->GetLeft()) / barLength;
					hsv.z = hsv.z * 2 - 1;
					deltaPos = deltaPos.Normalize();
					hsv.x = acosf(deltaPos.x) * (deltaPos.y > 0 ? 1 : -1) * 180.0 / M_PI;
					hsv.x = hsv.x * -1 + 90;
					if (hsv.z < 0) hsv.x += 180;
					if (hsv.x < 0.0) hsv.x += 360.0;
					if (hsv.x > 360.0) hsv.x -= 360.0;
					m_materialColorCorrection->m_uGamma = hsv2rgb(hsv);
				}

				// gain
				isChanged = false;
				if (m_gain_UIColorWheel->CheckMouseInside()) {
					isChanged = true;
					m_gain_UIColorTarget->SetPosition(Vector3(mousePosX, mousePosY, m_gain_UIColorTarget->GetPosition().z));
				}
				if (m_gain_UIBar->CheckMouseInside()) {
					isChanged = true;
					m_gain_UIHandle->SetPosition(Vector3(mousePosX, m_gain_UIBar->GetPosition().y, m_gain_UIBar->GetPosition().z));
				}
				if (isChanged) {
					m_isPressingToggle = true;
					deltaPos.x = m_gain_UIColorTarget->GetPosition().x - m_gain_UIColorWheel->GetPosition().x;
					deltaPos.y = m_gain_UIColorTarget->GetPosition().y - m_gain_UIColorWheel->GetPosition().y;
					hsv.y = deltaPos.Length() / colorWheelRadius;
					hsv.z = (m_gain_UIHandle->GetPosition().x - m_gain_UIBar->GetLeft()) / barLength;
					hsv.z = hsv.z * 2 - 1;
					deltaPos = deltaPos.Normalize();
					hsv.x = acosf(deltaPos.x) * (deltaPos.y > 0 ? 1 : -1) * 180.0 / M_PI;
					hsv.x = hsv.x * -1 + 90;
					if (hsv.z < 0) hsv.x += 180;
					if (hsv.x < 0.0) hsv.x += 360.0;
					if (hsv.x > 360.0) hsv.x -= 360.0;
					m_materialColorCorrection->m_uGain = hsv2rgb(hsv);
				}

				// offset
				isChanged = false;
				if (m_offset_UIColorWheel->CheckMouseInside()) {
					isChanged = true;
					m_offset_UIColorTarget->SetPosition(Vector3(mousePosX, mousePosY, m_offset_UIColorTarget->GetPosition().z));
				}
				if (m_offset_UIBar->CheckMouseInside()) {
					isChanged = true;
					m_offset_UIHandle->SetPosition(Vector3(mousePosX, m_offset_UIBar->GetPosition().y, m_offset_UIBar->GetPosition().z));
				}
				if (isChanged) {
					m_isPressingToggle = true;
					deltaPos.x = m_offset_UIColorTarget->GetPosition().x - m_offset_UIColorWheel->GetPosition().x;
					deltaPos.y = m_offset_UIColorTarget->GetPosition().y - m_offset_UIColorWheel->GetPosition().y;
					hsv.y = deltaPos.Length() / colorWheelRadius;
					hsv.z = (m_offset_UIHandle->GetPosition().x - m_offset_UIBar->GetLeft()) / barLength;
					hsv.z = hsv.z * 2 - 1;
					deltaPos = deltaPos.Normalize();
					hsv.x = acosf(deltaPos.x) * (deltaPos.y > 0 ? 1 : -1) * 180.0 / M_PI;
					hsv.x = hsv.x * -1 + 90;
					if (hsv.z < 0) hsv.x += 180;
					if (hsv.x < 0.0) hsv.x += 360.0;
					if (hsv.x > 360.0) hsv.x -= 360.0;
					m_materialColorCorrection->m_uOffset = hsv2rgb(hsv);
				}

				float value;
				barLength = (m_contrast_UIBar->GetRight() - m_contrast_UIBar->GetLeft()) * m_contrast_UIBar->GetScale().x;
				if (m_contrast_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_contrast_UIHandle->SetPosition(Vector3(mousePosX, m_contrast_UIBar->GetPosition().y, m_contrast_UIBar->GetPosition().z));
					value = (m_contrast_UIHandle->GetPosition().x - m_contrast_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uContrast = value * 2;
				}
				if (m_contrastPivot_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_contrastPivot_UIHandle->SetPosition(Vector3(mousePosX, m_contrastPivot_UIBar->GetPosition().y, m_contrastPivot_UIBar->GetPosition().z));
					value = (m_contrastPivot_UIHandle->GetPosition().x - m_contrastPivot_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uContrastPivot = value;
				}
				if (m_saturation_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_saturation_UIHandle->SetPosition(Vector3(mousePosX, m_saturation_UIBar->GetPosition().y, m_saturation_UIBar->GetPosition().z));
					value = (m_saturation_UIHandle->GetPosition().x - m_saturation_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uSaturation = value * 2;
				}
				if (m_hue_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_hue_UIHandle->SetPosition(Vector3(mousePosX, m_hue_UIBar->GetPosition().y, m_hue_UIBar->GetPosition().z));
					value = (m_hue_UIHandle->GetPosition().x - m_hue_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uHue = (value * 2 - 1) * M_PI;
				}
				if (m_temperature_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_temperature_UIHandle->SetPosition(Vector3(mousePosX, m_temperature_UIBar->GetPosition().y, m_temperature_UIBar->GetPosition().z));
					value = (m_temperature_UIHandle->GetPosition().x - m_temperature_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uTemperature = (value > 0.5) ? ((value - 0.5) * 2 * (150 - 66) + 66) : (value * 2 * (66 - 15) + 15);
				}
				if (m_unsharpMask_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_unsharpMask_UIHandle->SetPosition(Vector3(mousePosX, m_unsharpMask_UIBar->GetPosition().y, m_unsharpMask_UIBar->GetPosition().z));
					value = (m_unsharpMask_UIHandle->GetPosition().x - m_unsharpMask_UIBar->GetLeft()) / barLength;
					m_materialColorCorrection->m_uUnsharpMask = value * 5;
				}

				barLength = (m_vignetteExtend_UIBar->GetRight() - m_vignetteExtend_UIBar->GetLeft()) * m_vignetteExtend_UIBar->GetScale().x;
				if (m_vignetteExtend_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_vignetteExtend_UIHandle->SetPosition(Vector3(mousePosX, m_vignetteExtend_UIBar->GetPosition().y, m_vignetteExtend_UIBar->GetPosition().z));
					value = (m_vignetteExtend_UIHandle->GetPosition().x - m_vignetteExtend_UIBar->GetLeft()) / barLength;
					m_materialVignette->m_uExtend = value * 2;
				}
				if (m_vignetteIntensify_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_vignetteIntensify_UIHandle->SetPosition(Vector3(mousePosX, m_vignetteIntensify_UIBar->GetPosition().y, m_vignetteIntensify_UIBar->GetPosition().z));
					value = (m_vignetteIntensify_UIHandle->GetPosition().x - m_vignetteIntensify_UIBar->GetLeft()) / barLength;
					m_materialVignette->m_uIntensify = value * 50;
				}
				if (m_vignetteBrightness_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_vignetteBrightness_UIHandle->SetPosition(Vector3(mousePosX, m_vignetteBrightness_UIBar->GetPosition().y, m_vignetteBrightness_UIBar->GetPosition().z));
					value = (m_vignetteBrightness_UIHandle->GetPosition().x - m_vignetteBrightness_UIBar->GetLeft()) / barLength;
					m_materialVignette->m_uColor.x = value;
					m_materialVignette->m_uColor.y = value;
					m_materialVignette->m_uColor.z = value;
				}
				if (m_vignetteOpacity_UIBar->CheckMouseInside()) {
					m_isPressingToggle = true;
					m_vignetteOpacity_UIHandle->SetPosition(Vector3(mousePosX, m_vignetteOpacity_UIBar->GetPosition().y, m_vignetteOpacity_UIBar->GetPosition().z));
					value = (m_vignetteOpacity_UIHandle->GetPosition().x - m_vignetteOpacity_UIBar->GetLeft()) / barLength;
					m_materialVignette->m_uColor.w = value;
				}
			}
		}
	}

	if (m_isUseUI) Singleton<InputManager>::GetInstance()->m_isLocked = true;
}

void PostProcessingManager::SetShowUI(bool value)
{
	if (m_isUseUI != value) {
		m_isUseUI = value;
		ShowCursor(value);
	}
}

bool PostProcessingManager::CheckIsShowUI()
{
	return (m_isEnabled && m_isUseUI && m_turnOnColorCorrectionDebug);
}
