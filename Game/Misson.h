#pragma once
#include "UIText.h"
#include <vector>
class Misson
{
private:
	int map;
	std::vector<UIText*> misson;

	int* m_listMisson;
	int m_numberOfStar;

	int m_kill, m_MaxKill, m_EnemyType;
	int m_Score, m_MaxScore;
	bool m_Blood;
public:
	Misson(int map);
	~Misson();
	void Render();
	void loadMisson(char* dataMisson);
	int* getListMisson() { return m_listMisson; }
	int getEnemyType() { return m_EnemyType; }
	void countKill();
	void setBlood(bool p) { this->m_Blood = p; }
	void setScore(int score) { this->m_Score = score; }
	int countStar();
};

