#pragma once
#include "Button.h"
#define REACT_UNIT 0
#define TRIAGLE_UNIT 1

#define OBSTACLE_UNIT 0
#define SUICIDEBUG_UNIT 2
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
private:
	void getInformation();
	UnitInfor m_infor;
	UnitInfor** buffer;
};

