#pragma once
#include "Sprite.h"
#include <vector>
#include "../Framework3D/TrainingFramework/framework3d.h"
class MapSection {
private:
	int m_id;
	std::vector<Sprite*> m_listObject;
	float m_difficulty;
	float m_startHeight, m_endHeight;
public:
	MapSection();
	~MapSection();
	void Clear();
	void Generate(int id, float difficulty, float startHeight, float endHeight);
	void Update(float deltaTime);
	void Render(Camera2D* camera);

	float GetStartHeight();
	float GetEndHeight();

	void AddObject(Sprite* obj);
};