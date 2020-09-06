#pragma once
#include "Sprite.h"
#include <vector>
#include "../Framework3D/TrainingFramework/framework3d.h"
class MapSection {
private:
	int m_id;
	std::vector<Sprite*> m_listObject;
	float m_difficulty;
	float m_bottom, m_top, m_left, m_right;
public:
	MapSection();
	~MapSection();
	void Clear();
	void GenerateVoid(int id, float bottom, float top, float left, float right);
	void Generate(int id, float difficulty, float bottom, float top, float left, float right, int iNumOfRoute, int iThemeId, int iThemeId2 = -1);
	void Update(float deltaTime);
	void Render(Camera2D* camera);

	float GetBottom();
	float GetTop();

	void AddObject(Sprite* obj);
};