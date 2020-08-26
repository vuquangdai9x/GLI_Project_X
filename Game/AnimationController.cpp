#include "AnimationController.h"
#include "ResourcesManager2D.h"

AnimationController::AnimationController()
{
	m_currentAnimState = m_defaultAnimState = NULL;
	m_timeCounter = -1;
	m_iLoopCounter = -1;
	m_offsetX = m_offsetY = 0;
	m_subWidth = m_subHeight = m_frameWidth = m_frameHeight = 1;
}

AnimationController::~AnimationController()
{
	for (int i = 0;i < m_listAnimState.size();i++) {
		if (m_listAnimState[i].listTexture != NULL) {
			delete[] m_listAnimState[i].listTexture;
		}
	}
}

void AnimationController::Clone(AnimationController& originAnimCtrl) {
	m_listAnimState.clear();
	for (int i = 0; i < originAnimCtrl.m_listAnimState.size();i++) {
		if (originAnimCtrl.m_listAnimState[i].isUseList) {
			int iNumOfTexture = originAnimCtrl.m_listAnimState[i].iNumOfTexture;
			Texture** listTexture = new Texture * [iNumOfTexture];
			for (int j = 0;j < iNumOfTexture;j++) {
				listTexture[j] = originAnimCtrl.m_listAnimState[i].listTexture[j];
			}
			m_listAnimState.push_back(AnimState{
					originAnimCtrl.m_listAnimState[i].id,
					true,
					NULL,
					iNumOfTexture,
					listTexture,
					0, 0,
					originAnimCtrl.m_listAnimState[i].cycleTime
				});
		}
		else {
			m_listAnimState.push_back(AnimState{
				originAnimCtrl.m_listAnimState[i].id,
				false,
				originAnimCtrl.m_listAnimState[i].texture,
				0,
				NULL,
				originAnimCtrl.m_listAnimState[i].iSplitX,
				originAnimCtrl.m_listAnimState[i].iSplitY,
				originAnimCtrl.m_listAnimState[i].cycleTime
			});
		}
	}
	if (originAnimCtrl.m_defaultAnimState != NULL)
		SetDefaultAnimState(originAnimCtrl.m_defaultAnimState->id);
}

void AnimationController::AddAnimState(int iAnimId, int iTextureId, int iSplitX, int iSplitY, float cycleTime)
{
	Texture* texture = Singleton<ResourceManager2D>::GetInstance()->GetTexture(iTextureId);
	if (texture == NULL) {
		printf("[ERR] AnimationController:AddAnimState: Can not add Animation. Texture is NULL\n");
		return;
	}
	if (iSplitX <= 0 || iSplitY <= 0) {
		printf("[ERR] AnimationController:AddAnimState: Split number is less or equal zero\n");
		return;
	}
	if (cycleTime <=0 ) {
		printf("[ERR] AnimationController:AddAnimState: Cycle time is less or equal zero\n");
		return;
	}
	for (int i = 0;i < m_listAnimState.size();i++) {
		if (iAnimId == m_listAnimState[i].id) {
			printf("[Wrn] AnimationController:AddAnimState: AnimId existed. This can cause trouble in the future\n");
			break;
		}
	}
	m_listAnimState.push_back(AnimState{ iAnimId,
		false,
		texture,
		0,
		NULL,
		iSplitX, iSplitY,
		cycleTime });
}

void AnimationController::AddAnimState(int iAnimId, int iNumOfTexture, int listTexId[], float cycleTime)
{
	if (iNumOfTexture <= 0) {
		printf("[ERR] AnimationController:AddAnimState: Number of texture is zero\n");
		return;
	}
	int iNumOfTextureValid = 0;
	Texture * *listTexture = new Texture * [iNumOfTexture];
	for (int i = 0;i < iNumOfTexture;i++) {
		listTexture[iNumOfTextureValid] = Singleton<ResourceManager2D>::GetInstance()->GetTexture(listTexId[i]);
		if (listTexture[iNumOfTextureValid] != NULL)
			iNumOfTextureValid++;
	}
	if (cycleTime <= 0) {
		printf("[ERR] AnimationController:AddAnimState: Cycle time is less or equal zero\n");
		return;
	}
	for (int i = 0;i < m_listAnimState.size();i++) {
		if (iAnimId == m_listAnimState[i].id) {
			printf("[Wrn] AnimationController:AddAnimState: AnimId existed. This can cause trouble in the future\n");
			break;
		}
	}
	m_listAnimState.push_back(AnimState{
		iAnimId,
		true,
		NULL,
		iNumOfTextureValid,
		listTexture,
		0, 0,
		cycleTime });
}

void AnimationController::RunAnimState(int iAnimId, int iLoopCount)
{
	bool isAnimExist = false;
	for (int i = 0;i < m_listAnimState.size();i++) {
		if (iAnimId == m_listAnimState[i].id) {
			isAnimExist = true;
			m_currentAnimState = &(m_listAnimState[i]);
			if (m_currentAnimState->isUseList) {
				m_subHeight = 1;
				m_subWidth = 1;
				m_offsetX = m_offsetY = 0;
				m_frameWidth = m_currentAnimState->listTexture[0]->GetWidth() / Globals::pixelPerUnit;
				m_frameHeight = m_currentAnimState->listTexture[0]->GetHeight() / Globals::pixelPerUnit;
			}
			else {
				m_subHeight = 1.0 / m_currentAnimState->iSplitY;
				m_subWidth = 1.0 / m_currentAnimState->iSplitX;
				m_frameWidth = m_currentAnimState->texture->GetWidth() / Globals::pixelPerUnit / (float)m_currentAnimState->iSplitX;
				m_frameHeight = m_currentAnimState->texture->GetHeight() / Globals::pixelPerUnit / (float)m_currentAnimState->iSplitY;
			}
			m_timeCounter = m_currentAnimState->cycleTime;
			m_iLoopCounter = iLoopCount;
			break;
		}
	}
	if (!isAnimExist) {
		printf("[Wrn] AnimationController:RunAnimState: There no animation has id %d\n", iAnimId);
	}
}

void AnimationController::SetDefaultAnimState(int iAnimId)
{
	bool isAnimExist = false;
	for (int i = 0;i < m_listAnimState.size();i++) {
		if (iAnimId == m_listAnimState[i].id) {
			isAnimExist = true;
			m_defaultAnimState = &(m_listAnimState[i]);
			if (m_currentAnimState == NULL)
				RunAnimState(iAnimId,-1);
			break;
		}
	}
	if (!isAnimExist) {
		printf("[Wrn] AnimationController:SetDefaultAnimState: There no animation has id %d\n", iAnimId);
	}
}

//float AnimationController::GetOffsetX()
//{
//	return m_offsetX;
//}
//
//float AnimationController::GetOffsetY()
//{
//	return m_offsetY;
//}
//
//float AnimationController::GetSubWidth()
//{
//	return m_subWidth;
//}
//
//float AnimationController::GetSubHeight()
//{
//	return m_subHeight;
//}

Texture* AnimationController::GetTexture()
{
	if (m_currentAnimState == NULL) return NULL;
	if (m_currentAnimState->isUseList) {
		return m_currentAnimState->listTexture[(int)((1 - m_timeCounter / m_currentAnimState->cycleTime) * m_currentAnimState->iNumOfTexture)];
	}
	else {
		return m_currentAnimState->texture;
	}
}

void AnimationController::UpdateAnim(float deltaTime)
{
	if (m_currentAnimState == NULL) return;
	m_timeCounter -= deltaTime;
	if (m_timeCounter <= 0) {
		if (m_iLoopCounter < 0) { // infinite loop
			m_timeCounter = m_currentAnimState->cycleTime;
		}
		else {
			m_iLoopCounter--;
			if (m_iLoopCounter == 0) { 
				// set default state
				if (m_defaultAnimState == NULL) {
					printf("[Wrn] AnimationController:UpdateAnim: Can get default animation. Continue run current animation infinitely\n");
				}
				else {
					m_currentAnimState = m_defaultAnimState;
					if (m_currentAnimState->isUseList) {
						m_subHeight = 1;
						m_subWidth = 1;
						m_offsetX = m_offsetY = 0;
					}
					else {
						m_subHeight = 1.0 / m_currentAnimState->iSplitX;
						m_subWidth = 1.0 / m_currentAnimState->iSplitY;
						m_frameWidth = m_currentAnimState->texture->GetWidth() / Globals::pixelPerUnit / (float)m_currentAnimState->iSplitX;
						m_frameHeight = m_currentAnimState->texture->GetHeight() / Globals::pixelPerUnit / (float)m_currentAnimState->iSplitY;
					}
					m_timeCounter = m_currentAnimState->cycleTime;
					m_iLoopCounter = -1;
				}
			}
			else {
				m_timeCounter = m_currentAnimState->cycleTime;
			}
		}
	}
	int iFrameIndex;
	if (m_currentAnimState->isUseList) {
		iFrameIndex = (int)((1 - m_timeCounter / m_currentAnimState->cycleTime) * m_currentAnimState->iNumOfTexture);
		m_frameWidth = m_currentAnimState->listTexture[iFrameIndex]->GetWidth() / Globals::pixelPerUnit;
		m_frameHeight = m_currentAnimState->listTexture[iFrameIndex]->GetHeight() / Globals::pixelPerUnit;
	}
	else {
		iFrameIndex = ((1 - m_timeCounter / m_currentAnimState->cycleTime) * m_currentAnimState->iSplitX * m_currentAnimState->iSplitY);
		m_offsetX = (iFrameIndex % m_currentAnimState->iSplitX) * m_subWidth;
		m_offsetY = (iFrameIndex / m_currentAnimState->iSplitX) * m_subHeight;
	}
}
