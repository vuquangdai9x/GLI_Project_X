#include "PostProcessingManager.h"
#include "../Framework3D/TrainingFramework/framework3d.h"


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

PostProcessingManager::PostProcessingManager()
{
	// color correction
	m_isEnabledColorCorrection = m_canRunColorCorrection = true;
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
}


PostProcessingManager::~PostProcessingManager()
{
	for (int i = 0; i < m_listMaterial.size(); i++) {
		delete m_listMaterial[i];
	}
	for (int i = 0; i < m_listShader.size(); i++) {
		delete m_listShader[i];
	}
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
		m_canRunColorCorrection = false;
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

void PostProcessingManager::Prepare()
{
	if (m_isEnabled && m_canRun) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufObjColorCorrection);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::TAB)) {
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
	else {
		int adjust = Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::LSHIFT) > 0 ? 1 : -1;
		m_materialColorCorrection->m_uContrast += 0.01 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N1);
		if (m_materialColorCorrection->m_uContrast >= 1.99) m_materialColorCorrection->m_uContrast = 1.99;
		else if (m_materialColorCorrection->m_uContrast <= 0.01) m_materialColorCorrection->m_uContrast = 0.01;

		m_materialColorCorrection->m_uContrastPivot += 0.01 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N2);
		if (m_materialColorCorrection->m_uContrastPivot >= 0.99) m_materialColorCorrection->m_uContrastPivot = 0.99;
		else if (m_materialColorCorrection->m_uContrastPivot <= 0.01) m_materialColorCorrection->m_uContrastPivot = 0.01;

		m_materialColorCorrection->m_uSaturation += 0.05 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N3);

		m_materialColorCorrection->m_uHue += 0.01 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N4);

		m_materialColorCorrection->m_uTemperature += 0.5 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N5);
		if (m_materialColorCorrection->m_uTemperature >= 150) m_materialColorCorrection->m_uTemperature = 150;
		else if (m_materialColorCorrection->m_uTemperature <= 20) m_materialColorCorrection->m_uTemperature = 20;

		m_materialColorCorrection->m_uUnsharpMask += 0.01 * adjust * Singleton<InputManager>::GetInstance()->GetBit(InputManager::Key::N6);
		if (m_materialColorCorrection->m_uUnsharpMask <= 0) m_materialColorCorrection->m_uUnsharpMask = 0;
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
}
