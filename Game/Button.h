#pragma once
#include "Sprite.h"
class Button : public Sprite
{
private:
	float x, y;
	float width, height;
public:
	Button();
	~Button();
	void Update(float deltaTime);
};

