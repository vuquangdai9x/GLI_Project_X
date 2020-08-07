#pragma once
#include "stdafx.h"

class Shaders 
{
public:
	Shaders(int id);
public:
	int m_id;
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	virtual bool Init(char * fileVertexShader, char * fileFragmentShader);
	virtual ~Shaders();
};