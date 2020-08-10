#pragma once
#include "stdafx.h"

class InputManager {
public:
	InputManager();
	~InputManager() {};

private:
	// NOTE: if more than 31 key, use: unsigned long int m_mask
	unsigned int m_mask = 0;
public:
	enum Key { NONE, A, D, W, S, LSHIFT, SPACE, UP, DOWN, LEFT, RIGHT, N1, N2, N3, N4, Q, E, TAB, Z, X, C };
	void KeyPressed(unsigned char key, bool isKeyPressed);
	int GetBit(Key key);

	void Update(float deltaTime);

};