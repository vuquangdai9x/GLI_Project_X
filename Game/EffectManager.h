#pragma once
#pragma warning(disable: 4996)

#include <vector>
#include "Singleton.h"
#include "MaterialParticle2D.h"
#include "EffectCompositePool.h"

#include "ParticlesEmitter.h"

class EffectManager {
private:
	std::vector<Shaders*> m_listShader;
	std::vector<MaterialParticle2D*> m_listMaterialParticle;
	std::vector<EffectCompositePool*> m_listEffectCompositePool;

	void HexColorToVec4(Vector4& color, unsigned uiHexColor, float alpha);
	MaterialParticle2D* GetMaterial(int iMaterialId);
	Shaders* GetShader(int iShaderId);
public:
	ParticlesEmitter* m_emitter = NULL;

	EffectManager();
	~EffectManager();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
	void LoadEffect(char* effectFilePath);

	void CreateParticlesSystem(Vector3 position, int effectId, float rotation = 0);
};