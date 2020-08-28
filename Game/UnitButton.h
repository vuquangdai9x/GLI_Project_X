#pragma once
#include "Button.h"
struct UnitInfor {
	int type;
	int boxType;
	int mainTex;
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

