#include "HUDController.h"

void HUDController::Init(float healthBarScale, UIComponent* healthIcon, UIComponent* healthBar, UIComponent* healthDecorate,
	UIText* bulletStatus, UIComponent* weaponIcon, UIComponent* targetIcon, UIText* score)
{
	m_healthBarScale = healthBarScale;
	m_healthIcon = healthIcon;
	m_healthBar = healthBar;
	m_healthDecorate = healthDecorate;
	m_bulletStatus = bulletStatus;
	m_weaponIcon = weaponIcon;
	m_targetIcon = targetIcon;
	m_score = score;

	float healthBarTop = 0.95,
		healthBarBottom = 0.9;

	m_healthIcon->SetBound(
		healthBarTop,
		healthBarBottom,
		-1 + 0.05,
		-1 + 0.05 + 0.05
	);
	m_healthIcon->SetRenderType(UIComponent::RenderType::Fit);
	m_healthIcon->SetAlignHorizontal(UIComponent::AlignHorizontal::Right);

	m_healthBar->SetBound(
		healthBarTop,
		healthBarBottom,
		-1 + 0.05 + 0.05,
		- 1 + 0.05 + 1
	);
	m_healthBar->SetRenderType(UIComponent::RenderType::Stretch);
	m_healthBar->SetScale(Vector2(1.0, 0.25));

	m_healthDecorate->SetBound(
		healthBarTop,
		healthBarBottom,
		-1 + 0.05 + 1,
		-1 + 0.05 + 1 + 0.05
	);
	m_healthDecorate->SetRenderType(UIComponent::RenderType::Fit);
	m_healthDecorate->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);
	m_healthDecorate->SetScale(Vector2(0.5, 0.5));

	m_weaponIcon->SetBound(
		0.9,
		0.85,
		-1 + 0.05,
		-1 + 0.05 + 0.1
	);
	m_weaponIcon->SetRenderType(UIComponent::RenderType::FitHeight);
	m_weaponIcon->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);

	m_bulletStatus->SetBound(
		0.85,
		0.80,
		-1 + 0.05,
		-1 + 0.05 + 0.05
	);
	m_bulletStatus->SetRenderType(UIComponent::RenderType::FitHeight);
	m_bulletStatus->SetAlignHorizontal(UIComponent::AlignHorizontal::Left);

	m_score->SetBound(
		0.95,
		0.9,
		0.9,
		0.95
	);
	m_score->SetRenderType(UIComponent::RenderType::FitHeight);
	m_score->SetAlignHorizontal(UIComponent::AlignHorizontal::Right);
	m_score->SetScale(Vector2(0.8, 0.8));

	m_targetIcon->SetBound(
		0.05,
		0,
		0,
		0.05
	);
	m_targetIcon->SetRenderType(UIComponent::RenderType::Fit);
}

void HUDController::UpdateHealthBar(float currentHP, float maxHP)
{
	float healthBarRight = m_healthBar->GetLeft() + currentHP / m_healthBarScale;
	m_healthBar->SetRight(healthBarRight);
	float healthDecorateWidth = m_healthDecorate->GetRight() - m_healthDecorate->GetLeft();
	m_healthDecorate->SetLeft(healthBarRight);
	m_healthDecorate->SetRight(healthBarRight + healthDecorateWidth);
}

void HUDController::ChangeWeaponIcon(Texture* weaponTex)
{
	m_weaponIcon->SetMainTexture(weaponTex->GetId());
}

void HUDController::ChangeTargetIcon(Texture* targetTex)
{
	m_targetIcon->SetMainTexture(targetTex->GetId());
}

void HUDController::UpdateTargetPosition(float screenRatioPosX, float screenRatioPosY)
{
	Vector3 newPos = m_targetIcon->GetPosition();
	newPos.x = screenRatioPosX * 2 - 1;
	newPos.y = screenRatioPosY * 2 - 1;
	newPos.y *= -1;
	m_targetIcon->SetPosition(newPos);
}

void HUDController::UpdateScore(int iScore)
{
	char scoreText[20];
	snprintf(scoreText, sizeof(scoreText), "Score: %d", iScore);
	m_score->SetText(scoreText);
}

void HUDController::UpdateBulletStatus(int iCurrent, int iMax)
{
	char bulletStatusText[20];
	snprintf(bulletStatusText, sizeof(bulletStatusText), "%d / %d", iCurrent, iMax);
	m_bulletStatus->SetText(bulletStatusText);
}
