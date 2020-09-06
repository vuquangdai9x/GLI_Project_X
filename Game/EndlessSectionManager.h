#pragma once
#pragma warning(disable: 4996)
#include "MapSection.h"
#include "Player.h"
#include "DifficultyCurve.h"
#include <vector>

class EndlessSectionManager {
private:
	int m_iSectionIdCount;
	std::vector<MapSection*> m_listMapSection;
	int m_iPrevSectionIndex, m_iCurrSectionIndex, m_iNextSectionIndex;
	int m_iNextOffset, m_iPrevOffset;
	float m_triggerDelPrevOffset, m_triggerGenNextOffset;
	bool m_isDelPrev, m_isGenNext;

	Player* m_player;
	DifficultyCurveFnPtr m_difficultyCurve;
	float m_leftBound, m_rightBound;
	
	float m_currStartHeight, m_currEndHeight, m_genStartHeight;
public:
	EndlessSectionManager(Player* player, float leftBound, float rightBound);
	~EndlessSectionManager();
	void Load(FILE* fIn);
	void Update(float deltaTime);
	void Render(Camera2D* camera);
};