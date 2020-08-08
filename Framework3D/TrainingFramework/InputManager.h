#pragma once
#include "stdafx.h"

class InputManager {
protected:
	InputManager();
	~InputManager() {}
	InputManager(const InputManager &) {}
	InputManager& operator =(const InputManager &) {}
public:
	static void CreateInstance()
	{
		if (ms_pInstance == nullptr)
			ms_pInstance = new InputManager;
	}
	static InputManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = nullptr;
		}
	}

	void SomeFunction() {}

protected:
	static InputManager * ms_pInstance;

private:
	// NOTE: if more than 31 key, use: unsigned long int m_mask
	unsigned int m_mask = 0;
	enum Key {NONE,A,D,W,S,LSHIFT,SPACE,UP,DOWN,LEFT,RIGHT,N1,N2,N3,N4,Q,E,TAB,Z,X,C};

	//void SetBit(unsigned int mask, int offset);
	//bool CheckBit(unsigned int mask, int offset);
public:
	void KeyPressed(unsigned char key, bool isKeyPressed);
	void Update(float deltaTime);
};