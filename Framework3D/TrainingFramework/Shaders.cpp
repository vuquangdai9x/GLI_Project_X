#include "stdafx.h"
#include "Shaders.h"

Shaders::Shaders(int id) : m_id(id) {}

bool Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return false;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return false;
	}

	program = esLoadProgram(vertexShader, fragmentShader);
	
	return true;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}