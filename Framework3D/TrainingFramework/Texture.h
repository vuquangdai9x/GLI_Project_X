#pragma once
#include "stdafx.h"

class Texture {
protected:
	int m_id;
	GLuint m_textureHandle;
	bool m_isCubeTexture;
	float m_width, m_height;
public:
	Texture(int id, bool isCubeTexture);
	virtual ~Texture();

	enum TILING_MODE
	{
		CLAMP, REPEAT, MIRROR
	};

	GLuint GetTextureHandle();
	bool IsCubeTexture();
	bool LoadTextureFromFile(char* filePath, TILING_MODE tilingMode);
	bool LoadCubeTextureFromFile(char* filePath, TILING_MODE tilingMode);
	bool LoadCubeTextureFromSplittedFiles(char ** imageFiles, TILING_MODE tilingMode);
	int GetId() { return m_id; }
	float GetWidth();
	float GetHeight();
};