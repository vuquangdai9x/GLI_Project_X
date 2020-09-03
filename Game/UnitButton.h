#pragma once
#include "Button.h"
#define REACT_UNIT 0
#define TRIAGLE_UNIT 1

#define OBSTACLE_UNIT 0
#define SUICIDEBUG_UNIT 2
#define FLOATINGFISH_UNIT 1
#define ROCKY_UNIT 3
#define BAT_UNINT 4
#define DEADLYBIRD_UNIT 5
struct UnitInfor {
	int type;
	int boxType;
	int mainTex;
	int id;
};
class UnitButton :public Button
{
public:
	void setInformation(UnitInfor infor);
	void setBuffer(UnitInfor** buf);
	void Update(float deltaTime);
	UnitButton(int id);
	UnitInfor m_infor;
private:
	void getInformation();
	UnitInfor** buffer;
};

