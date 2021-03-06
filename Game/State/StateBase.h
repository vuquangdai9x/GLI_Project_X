#pragma once

class StateBase
{
public:
	virtual bool Create() =0;
	virtual bool Release()=0;

	virtual void Render() = 0;
	virtual void Update(float deltaTime) = 0;

	virtual void KeyPress() = 0;
	//virtual void Mouse() = 0;
};

