#pragma once
#include "stdafx.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tangent;
	Vector2 uv;
	Vector4 color;

	void SetColorFromHex(unsigned int hexColor, float alpha);
	void SetColor(float r, float g, float b, float a);
	void SetPosition(float x, float y, float z);
	void SetNorm(float x, float y, float z);
	void SetBinorm(float x, float y, float z);
	void SetTangent(float x, float y, float z);
	void SetUV(float u, float v);
	void PrintInfo();
};