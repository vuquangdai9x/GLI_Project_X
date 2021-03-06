#include "EffectManager.h"
#include "ParticlesEmitter.h"
#include "ParticlesEmitterBox.h"
#include "ResourcesManager2D.h"
#include "CurveFunction.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
	for (int i = 0; i < m_listShader.size(); i++) {
		delete m_listShader[i];
	}
	for (int i = 0; i < m_listMaterialParticle.size(); i++) {
		delete m_listMaterialParticle[i];
	}
	for (int i = 0; i < m_listEffectCompositePool.size(); i++) {
		delete m_listEffectCompositePool[i];
	}
}

void EffectManager::Update(float deltaTime) {
	for (int i = 0;i < m_listEffectCompositePool.size();i++) {
		m_listEffectCompositePool[i]->Update(deltaTime);
	}
}
void EffectManager::Render(Camera2D* mainCamera) {
	for (int i = 0;i < m_listEffectCompositePool.size();i++) {
		m_listEffectCompositePool[i]->Render(mainCamera);
	}
}
void EffectManager::LoadEffect(char* effectFilePath) {
	const char* resourceDir = Globals::resourceDir;

	// NOTE: length of file path can be not enough and cause error
	char fileName[256];
	char filePath[512];

	strcpy(filePath, resourceDir);
	strcat(filePath, effectFilePath);
	FILE* fIn = fopen(filePath, "r");
	if (fIn == nullptr) {
		printf("[ERR] EffectManager::Init: Fails to load list effect file\n");
		return;
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
			printf("[msg] EffectManager::Init: Loaded Shader %d :\n\tVS: %s\n\tFS: %s\n", iShaderId, vertexShaderFile, fragmentShaderFile);
		}
		else {
			printf("[ERR] EffectManager::Init: Failed to load Shader \n\tVS: %s\n\tFS: %s\n", vertexShaderFile, fragmentShaderFile);
			delete shader;
		}
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
		if (strcmp("PARTICLES", materialType) == 0) {
			MaterialParticle2D* pMaterial;
			int iShaderId;
			pMaterial = new MaterialParticle2D(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			if (pMaterial->Init(GetShader(iShaderId))) {
				m_listMaterialParticle.push_back(pMaterial);
				printf("[msg] EffectManager::Init: Loaded Particles Material %d : Shader = %d\n", iMaterialId, iShaderId);
			}
			else {
				printf("[ERR] EffectManager::Init: Failed to load Particles Material %d : Shader = %d\n", iMaterialId, iShaderId);
				delete pMaterial;
			}
		}
		else if (strcmp("PARTICLES_BOX", materialType) == 0) {
			MaterialParticleBox* pMaterial;
			int iShaderId;
			pMaterial = new MaterialParticleBox(iMaterialId);
			fscanf(fIn, "SHADER %d\n", &iShaderId);
			if (pMaterial->Init(GetShader(iShaderId))) {
				m_listMaterialParticleBox.push_back(pMaterial);
				printf("[msg] EffectManager::Init: Loaded Particles Box Material %d : Shader = %d\n", iMaterialId, iShaderId);
			}
			else {
				printf("[ERR] EffectManager::Init: Failed to load Particles Box Material %d : Shader = %d\n", iMaterialId, iShaderId);
				delete pMaterial;
			}
		}
		else {
			printf("[ERR] EffectManager::Init: Material type invalid\n");
		}
	}

	//
	// Load Particles System
	//
	int iNumOfEffectComposite;
	int iEffectCompositeId;
	char effectName[256];
	int iNumOfEffect;
	fscanf(fIn, "\n#EffectComposites: %d\n", &iNumOfEffectComposite);
	for (int i = 0; i < iNumOfEffectComposite; i++) {
		fscanf(fIn, "\nID %d\n", &iEffectCompositeId);
		fscanf(fIn, "NAME %s\n", effectName);
		fscanf(fIn, "EFFECTS: %d\n", &iNumOfEffect);

		EffectComposite* templateEffComp = new EffectComposite(iEffectCompositeId);
		int iEffectValidCount = 0;
		for (int j = 0;j < iNumOfEffect;j++) {
			char effectType[50];
			Vector3 position;
			Vector2 scale;
			float rotation;

 			fscanf(fIn, "\nTYPE %s\n", effectType);
			fscanf(fIn, "POSITION %f %f %f\n", &(position.x), &(position.y), &(position.z));
			fscanf(fIn, "SCALE %f %f\n", &(scale.x), &(scale.y));
			fscanf(fIn, "ROTATION %f\n", &rotation);
			rotation = rotation * 2 * M_PI / 360;
			
			if (strcmp("PARTICLES_EMITTER", effectType)==0) {
				int iMaterialId, iTextureId;
				int iNumOfParticle;
				float cycleTime;
				int iNumOfLoop;
				ParticlesEmitter* emitter;
				Texture* texture;
				char curveName[30];

				fscanf(fIn, "\nMATERIAL %d\n", &iMaterialId);
				fscanf(fIn, "TEXTURE %d\n", &iTextureId);
				fscanf(fIn, "PARTICLES AMOUNT %d\n", &iNumOfParticle);
				fscanf(fIn, "CYCLE TIME %f\n", &cycleTime);
				fscanf(fIn, "LOOP %d\n", &iNumOfLoop);
				emitter = new ParticlesEmitter(position, scale, rotation);

				float emitAngle, emitAngleRandomRange;
				ParticlesEmitter::EmitType emitType;
				char emitTypeName[20];
				fscanf(fIn, "EMIT ANGLE %f\n", &emitAngle);
				emitAngle *= (M_PI / 180);
				fscanf(fIn, "EMIT ANGLE RANDOM %f\n", &emitAngleRandomRange);
				emitAngleRandomRange *= (M_PI / 180);
				fscanf(fIn, "EMIT TYPE %s\n", emitTypeName);
				if (strcmp("PURE_RANDOM", emitTypeName) == 0) {
					emitType = ParticlesEmitter::EmitType::PureRandom;
				}
				else if (strcmp("DISTRIBUTED", emitTypeName) == 0) {
					emitType = ParticlesEmitter::EmitType::Distributed;
				}
				else {
					printf("[ERR] EffectManager::Init: Emit type invalid : %s\n", emitTypeName);
				}
				emitter->SetAngleInfo(emitAngle, emitAngleRandomRange, emitType);

				float initValue, endValue, offsetRandomValue, mulRandomValue;
				fscanf(fIn, "RADIUS INIT %f\n", &initValue);
				fscanf(fIn, "RADIUS END %f\n", &endValue);
				fscanf(fIn, "RADIUS RANDOM OFFSET %f\n", &offsetRandomValue);
				fscanf(fIn, "RADIUS RANDOM MUL %f\n", &mulRandomValue);
				fscanf(fIn, "RADIUS CURVE %s\n", curveName);
				emitter->SetRadiusInfo(initValue, offsetRandomValue, mulRandomValue, endValue, CurveFunction::GetFunctionPtr(curveName));
				
				fscanf(fIn, "SIZE INIT %f\n", &initValue);
				fscanf(fIn, "SIZE END %f\n", &endValue);
				fscanf(fIn, "SIZE RANDOM %f\n", &offsetRandomValue);
				fscanf(fIn, "SIZE CURVE %s\n", curveName);
				emitter->SetSizeInfo(initValue, offsetRandomValue, endValue, CurveFunction::GetFunctionPtr(curveName));
				
				unsigned int uiHexColorInit, uiHexColorEnd, uiHexColorOffsetRandom;
				fscanf(fIn, "COLOR INIT %x %f\n", &uiHexColorInit, &initValue);
				fscanf(fIn, "COLOR END %x %f\n", &uiHexColorEnd, &endValue);
				fscanf(fIn, "COLOR RANDOM %x %f\n", &uiHexColorOffsetRandom, &offsetRandomValue);
				fscanf(fIn, "COLOR CURVE %s\n", curveName);
				Vector4 colorInit, colorEnd, colorOffsetRandom;
				HexColorToVec4(colorInit, uiHexColorInit, initValue);
				HexColorToVec4(colorEnd, uiHexColorEnd, endValue);
				HexColorToVec4(colorOffsetRandom, uiHexColorOffsetRandom, offsetRandomValue);
				emitter->SetColorInfo(colorInit, colorOffsetRandom, colorEnd, CurveFunction::GetFunctionPtr(curveName));
				
				texture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iTextureId);
				if (emitter->Init(GetMaterial(iMaterialId), texture, iNumOfParticle, cycleTime, iNumOfLoop)) {
					templateEffComp->AddEffect(emitter);
					iEffectValidCount++;
					printf("[msg] EffectManager::Init: Done Init Emitter %d : %s\n", j, effectType);
				}
				else {
					delete emitter;
					printf("[ERR] EffectManager::Init: Failed to init Emitter %d : %s\n", j, effectType);
				}
			}else if (strcmp("PARTICLES_EMITTER_BOX", effectType) == 0) {
				int iMaterialId, iTextureId;
				int iNumOfParticle;
				float cycleTime;
				int iNumOfLoop;
				ParticlesEmitterBox *emitter;
				Texture* texture;
				char curveName[30];

				fscanf(fIn, "\nMATERIAL %d\n", &iMaterialId);
				fscanf(fIn, "TEXTURE %d\n", &iTextureId);
				fscanf(fIn, "PARTICLES AMOUNT %d\n", &iNumOfParticle);
				fscanf(fIn, "CYCLE TIME %f\n", &cycleTime);
				fscanf(fIn, "LOOP %d\n", &iNumOfLoop);
				emitter = new ParticlesEmitterBox(position, scale, rotation);

				Vector2 emitRange;
				ParticlesEmitterBox::EmitType emitType;
				char emitTypeName[20];
				fscanf(fIn, "EMIT RANGE %f %f\n", &emitRange.x, &emitRange.y);
				fscanf(fIn, "EMIT TYPE %s\n", emitTypeName);
				if (strcmp("RANDOM", emitTypeName) == 0) {
					emitType = ParticlesEmitterBox::EmitType::PureRandom;
				}
				else if (strcmp("ACCUMULATIVE", emitTypeName) == 0) {
					emitType = ParticlesEmitterBox::EmitType::AccumulativeCenter;
				}
				else {
					printf("[ERR] EffectManager::Init: Emit type invalid : %s\n", emitTypeName);
				}
				emitter->SetEmitInfo(emitRange, emitType);

				/*
				SIZE INIT 10
				SIZE RANDOM 2
				COLOR INIT 75596F 1.0
				COLOR END E3CFE3 0.0
				COLOR RANDOM 010101 0.1
				COLOR CURVE EaseOutCirc*/
				
				Vector2 velocity, velocityRandomOffset;
				float velocityRandomMul;
				fscanf(fIn, "VELOCITY %f %f\n", &velocity.x, &velocity.y);
				fscanf(fIn, "VELOCITY RANDOM OFFSET %f %f\n", &velocityRandomOffset.x, &velocityRandomOffset.y);
				fscanf(fIn, "VELOCITY RANDOM MUL %f\n", &velocityRandomMul);
				emitter->SetVelocityInfo(velocityRandomOffset, velocityRandomMul);
				emitter->SetVelocity(velocity);

				float timeRandomOffset;
				float initValue, endValue, offsetRandomValue;
				fscanf(fIn, "TIME RANDOM OFFSET %f\n", &timeRandomOffset);
				fscanf(fIn, "TIME LIFE %f\n", &initValue);
				fscanf(fIn, "TIME LIFE RANDOM %f\n", &offsetRandomValue);
				emitter->SetTimeInfo(timeRandomOffset, initValue, offsetRandomValue);

				fscanf(fIn, "SIZE INIT %f\n", &initValue);
				fscanf(fIn, "SIZE RANDOM %f\n", &offsetRandomValue);
				emitter->SetSizeInfo(initValue, offsetRandomValue);

				unsigned int uiHexColorInit, uiHexColorEnd, uiHexColorOffsetRandom;
				fscanf(fIn, "COLOR INIT %x %f\n", &uiHexColorInit, &initValue);
				fscanf(fIn, "COLOR END %x %f\n", &uiHexColorEnd, &endValue);
				fscanf(fIn, "COLOR RANDOM %x %f\n", &uiHexColorOffsetRandom, &offsetRandomValue);
				fscanf(fIn, "COLOR CURVE %s\n", curveName);
				Vector4 colorInit, colorEnd, colorOffsetRandom;
				HexColorToVec4(colorInit, uiHexColorInit, initValue);
				HexColorToVec4(colorEnd, uiHexColorEnd, endValue);
				HexColorToVec4(colorOffsetRandom, uiHexColorOffsetRandom, offsetRandomValue);
				emitter->SetColorInfo(colorInit, colorOffsetRandom, colorEnd, CurveFunction::GetFunctionPtr(curveName));

				texture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iTextureId);
				if (emitter->Init(GetMaterialParticleBox(iMaterialId), texture, iNumOfParticle, cycleTime, iNumOfLoop)) {
					templateEffComp->AddEffect(emitter);
					iEffectValidCount++;
					printf("[msg] EffectManager::Init: Done Init Emitter %d : %s\n", j, effectType);
				}
				else {
					delete emitter;
					printf("[ERR] EffectManager::Init: Failed to init Emitter %d : %s\n", j, effectType);
				}

				int iNumOfSubTex;
				fscanf(fIn, "SUB TEXTURES %d:", &iNumOfSubTex);
				ParticlesEmitterBox* subEmitter;
				for (int subTexId = 0; subTexId < iNumOfSubTex; subTexId++) {
					subEmitter = new ParticlesEmitterBox(*emitter);
					fscanf(fIn, " %d", &iTextureId);
					texture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iTextureId);
					if (subEmitter->Init(GetMaterialParticleBox(iMaterialId), texture, iNumOfParticle, cycleTime, iNumOfLoop)) {
						templateEffComp->AddEffect(subEmitter);
						printf("[msg] EffectManager::Init: Done Init Sub Texture %d Of Emitter %d : %s\n", iTextureId, j, effectType);
					}
					else {
						delete subEmitter;
						printf("[ERR] EffectManager::Init: Failed to init Sub Texture %d Of Emitter %d : %s\n", iTextureId, j, effectType);
					}
				}
				fscanf(fIn, "\n");
			}
			else {
				printf("[ERR] EffectManager::Init: Invalid EffectType %d : %s\n", j, effectType);
			}
		}
		if (iEffectValidCount > 0) {
			m_listEffectCompositePool.push_back(new EffectCompositePool(iEffectCompositeId, 1, templateEffComp));
			printf("[msg] EffectManager::Init: Loaded EffectComposite %d: %s\n", iEffectCompositeId, effectName);
		}
		else {
			delete templateEffComp;
			printf("[ERR] EffectManager::Init: No valid effect in EffectComposite %d: %s\n", iEffectCompositeId, effectName);
		}
	}
	fclose(fIn);

	/*m_emitter = new ParticlesEmitter(Vector3(0, 5, 0), Vector2(1, 1), 0);
	m_emitter->Init(GetMaterial(0), 10, 10, 64, Vector4(1.0, 0.0, 0.0, 1.0), 3, 2);
	m_emitter->Play();*/
}

void EffectManager::HexColorToVec4(Vector4& color, unsigned uiHexColor, float alpha) {
	color.x = ((uiHexColor & 0xFF0000) >> 16) / (float)255;
	color.y = ((uiHexColor & 0x00FF00) >> 8) / (float)255;
	color.z = ((uiHexColor & 0x0000FF) >> 0) / (float)255;
	color.w = alpha;
}
MaterialParticle2D* EffectManager::GetMaterial(int iMaterialId) {
	for (int i = 0;i < m_listMaterialParticle.size();i++) {
		if (m_listMaterialParticle[i]->GetId() == iMaterialId) {
			return m_listMaterialParticle[i];
		}
	}
	return NULL;
}
MaterialParticleBox * EffectManager::GetMaterialParticleBox(int iMaterialId)
{
	for (int i = 0; i < m_listMaterialParticleBox.size(); i++) {
		if (m_listMaterialParticleBox[i]->GetId() == iMaterialId) {
			return m_listMaterialParticleBox[i];
		}
	}
	return NULL;
}
Shaders* EffectManager::GetShader(int iShaderId)
{
	for (int i = 0;i < m_listShader.size();i++) {
		if (m_listShader[i]->m_id == iShaderId) {
			return m_listShader[i];
		}
	}
	return NULL;
}
void EffectManager::CreateParticlesSystem(Vector3 position, int effectId, Vector2 scale, float rotation) {
	for (int i = 0;i < m_listEffectCompositePool.size();i++) {
		if (m_listEffectCompositePool[i]->GetId() == effectId) {
			m_listEffectCompositePool[i]->GetCompositeEffect()->Play(position, scale, rotation);
			break;
		}
	}
}

void EffectManager::DisableAll()
{
	for (int i = 0; i < m_listEffectCompositePool.size(); i++) {
		m_listEffectCompositePool[i]->DisableAll();
	}
}