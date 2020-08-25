#pragma once
#include "UIComponent.h"
#include "UIText.h"

class HUDController {
private:
	float m_healthBarScale; // 1 unit visualize how many HP
	UIComponent * m_healthIcon, * m_healthBar, * m_healthDecorate;
	UIText* m_bulletStatus;
	UIComponent* m_weaponIcon;
	UIComponent* m_targetIcon;
	UIText* m_score;
public:
	void Init(float healthBarScale, UIComponent* healthIcon, UIComponent* healthBar, UIComponent* healthDecorate,
		UIText* bulletStatus, UIComponent* weaponIcon, UIComponent* targetIcon, UIText* score);
	void UpdateHealthBar(float currentHP, float maxHP);
	void ChangeWeaponIcon(Texture* weaponTex);
	void ChangeTargetIcon(Texture* targetTex);
	void UpdateTargetPosition(float screenRatioPosX, float screenRatioPosY);
	void UpdateScore(int iScore);
	void UpdateBulletStatus(int iCurrent, int iMax);
};