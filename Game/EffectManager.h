#pragma once
#pragma warning(disable: 4996)

#include <vector>
#include "Singleton.h"
#include "MaterialParticle2D.h"
#include "MaterialParticleBox.h"
#include "EffectCompositePool.h"

class EffectManager {
private:
	std::vector<Shaders*> m_listShader;
	std::vector<MaterialParticle2D*> m_listMaterialParticle;
	std::vector<MaterialParticleBox*> m_listMaterialParticleBox;
	std::vector<EffectCompositePool*> m_listEffectCompositePool;

	void HexColorToVec4(Vector4& color, unsigned uiHexColor, float alpha);
	MaterialParticle2D* GetMaterial(int iMaterialId);
	MaterialParticleBox* GetMaterialParticleBox(int iMaterialId);
	Shaders* GetShader(int iShaderId);
public:
	EffectManager();
	~EffectManager();
	void Update(float deltaTime);
	void Render(Camera2D* mainCamera);
	void LoadEffect(char* effectFilePath);
	void DisableAll();
	void CreateParticlesSystem(Vector3 position, int effectId, Vector2 scale = Vector2(1,1), float rotation = 0);
};