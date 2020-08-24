#pragma once
#include "../Framework3D/TrainingFramework/framework3d.h"
#include <vector>
class AnimationController {
private:
	struct AnimState {
		int id;
		bool isUseList;
		Texture* texture;
		int iNumOfTexture;
		Texture** listTexture;
		int iSplitX, iSplitY;
		float cycleTime;
	};
	std::vector<AnimState> m_listAnimState;

	AnimState* m_currentAnimState;
	AnimState* m_defaultAnimState;

	float m_timeCounter;
	int m_iLoopCounter;
public:
	float m_offsetX, m_offsetY, m_subWidth, m_subHeight, m_frameWidth, m_frameHeight;
public:
	AnimationController();
	~AnimationController();
	void AddAnimState(int iAnimId, int iTextureId, int iSplitX, int iSplitY, float cycleTime);
	void AddAnimState(int iAnimId, int iNumOfTexture, int listTexId[], float cycleTime);
	void RunAnimState(int iAnimId, int iLoopCount);
	void SetDefaultAnimState(int iAnimId);
	/*float GetOffsetX();
	float GetOffsetY();
	float GetSubWidth();
	float GetSubHeight();*/
	Texture* GetTexture();
	void UpdateAnim(float deltaTime);
};