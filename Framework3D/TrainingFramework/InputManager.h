#pragma once
#include "stdafx.h"

class InputManager {
public:
	InputManager();
	~InputManager() {};

private:
	// NOTE: if more than 31 key, use: unsigned long int m_mask
	unsigned int m_mask = 0;
	int m_mouseEvent;
	int x, y, button;
public:
	bool m_isLocked;
	enum Key { NONE, A, D, W, S, LSHIFT, SPACE, UP, DOWN, LEFT, RIGHT, N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, Q, E, TAB, Z, X, C, ESCAPE, P, O };
	void KeyPressed(unsigned char key, bool isKeyPressed);
	int GetBit(Key key);
	void MouseEvent(int typeOfService, int button, int x, int y);
	int getMouseButton();
	int getMouseEvent();
	void getXY(int& xx, int& yy);
	void fixButton() { this->m_mouseEvent = MOUSE_RELEASE; }
};