#pragma once
#include "ResourcesManager2D.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "MaterialPostProcess.h"
#include "MaterialPPColorCorrection.h"
#include "Singleton.h"
#include <vector>
#include "UIComponent.h"
#include "UIText.h"

class PostProcessingManager
{
private:
	GLuint m_frameBufObjColorCorrection, m_depthBufObjColorCorrection;
	GLuint m_textureHandleColorCorrection;
	bool m_isEnabledColorCorrection, m_canRunColorCorrection;

	GLuint m_frameBufObj, m_depthBufObj;
	GLuint m_textureHandle;
	bool m_isEnabled, m_canRun;
	MaterialPostProcess* m_material;
	MaterialPPColorCorrection* m_materialColorCorrection;
	Model3D* m_model;

	std::vector<Shaders*> m_listShader;
	std::vector<MaterialPostProcess*> m_listMaterial;
	Shaders* GetShader(int iShaderId);
	MaterialPostProcess* GetMaterial(int iMaterialId);

	bool m_turnOnColorCorrectionDebug;
	UIComponent* m_lift_UIColorWheel, *m_lift_UIColorTarget, *m_lift_UIBar, *m_lift_UIHandle;
	UIComponent* m_gamma_UIColorWheel, *m_gamma_UIColorTarget, *m_gamma_UIBar, *m_gamma_UIHandle;
	UIComponent* m_gain_UIColorWheel, *m_gain_UIColorTarget, *m_gain_UIBar, *m_gain_UIHandle;
	UIComponent* m_offset_UIColorWheel, *m_offset_UIColorTarget, *m_offset_UIBar, *m_offset_UIHandle;
	UIComponent* m_contrast_UIBar, *m_contrast_UIHandle;
	UIComponent* m_contrastPivot_UIBar, *m_contrastPivot_UIHandle;
	UIComponent* m_saturation_UIBar, *m_saturation_UIHandle;
	UIComponent* m_hue_UIBar, *m_hue_UIHandle;
	UIComponent* m_temperature_UIBar, *m_temperature_UIHandle;
	UIComponent* m_unsharpMask_UIBar, *m_unsharpMask_UIHandle;
	std::vector<UIText*> m_listText;
	Camera2D* m_camera;

	float m_colorBlendUI_top, m_colorBlendUI_bottom, m_colorBlendUI_left, m_colorBlendUI_right;
	float m_colorCorrectionUI_top, m_colorCorrectionUI_bottom, m_colorCorrectionUI_left, m_colorCorrectionUI_right;
	void CreateUI();

	bool m_isUseUI;
	bool m_isPressingToggle;

	void ResetColorCorrection();
public:
	PostProcessingManager();
	~PostProcessingManager();
	void Load(char* postProcessFilePath);
	void SetMaterial(int iMaterialId);
	void Prepare();
	void Render();
	void Update(float deltaTime);
	void SetShowUI(bool value);
	bool CheckIsShowUI();
};

