#pragma once
#include "StateBase.h"
#include"../Button.h"
#include"../UnitButton.h"
#include "../Sprite.h"
#include"../Player.h"
#include"../MapBorder.h"
#include<vector>
class FloatingFishMoveSet :public Sprite {
public:
	FloatingFishMoveSet(int id);
	Sprite* fish;
	Vector2 distanceToFish;
	void Update(float deltaTime) {
		Vector3 pos = fish->GetPosition();
		pos.x += distanceToFish.x;
		pos.y += distanceToFish.y;
		this->SetPosition(pos);
	}
private:

};
class MapEditor : public StateBase
{
public:
	MapEditor();
	~MapEditor();
	bool Create();
	bool Release();
	void loadMap(char* dataSceneFile);
	void Render();
	void Update(float deltaTime);
	void addObject();
	void KeyPress();
	int saveCheck=0;
	void deleteSprite(Sprite* pointer);
	Sprite* checkInside(float x, float y);
	void pushSprite(Sprite* pointer);
	Sprite* cloneSprite(Sprite* source);
	FloatingFishMoveSet* createX();
	void saveFile(char* dataSceneFile);
	void loadAnimation(FILE* fIn);
	void pushBackBoxType(int texture, int type);
private:
	std::vector<UnitButton*> button;
	int m_mouseDragCheck = 0;
	int m_norColor = 0xffffff;
	int m_chooseColor = 0xffafff;
	Player* m_play;
	Button *saveButton;
	MapBorder* bgr, * borderLeft, * borderRight;
	Sprite *m_victoryPoint=NULL;
	std::vector<Sprite*> m_Obstacle, m_suicudeBug, m_floatingFish, m_rockyGolem, m_vampireBat, m_deadlyBird;
	bool m_floatingCreated = false;
	std::vector<int> m_floatingFishMoveNum;
	std::vector<Sprite*> m_fLoatingFishMove;
	std::vector<Sprite*> m_activeList;
	std::vector<float> m_distance_x, m_distance_y;
	std::vector<int> m_rectagle, m_triale;
	UnitInfor* m_curent;
	Sprite* m_currentSprite;
};


