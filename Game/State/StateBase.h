#pragma once

class StateBase
{
public:
	virtual bool Create() =0;
	virtual bool Release()=0;

	virtual void Render() = 0;
	virtual void Update() = 0;

	virtual void KeyPress(unsigned char key, bool bIsPressed) = 0;
	//virtual void Mouse() = 0;
};

