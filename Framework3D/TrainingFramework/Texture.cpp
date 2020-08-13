#include "stdafx.h"
#include "Texture.h"

Texture::Texture(int id, bool isCubeTexture) : m_id(id), m_isCubeTexture(isCubeTexture) {}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureHandle);
}

GLuint Texture::GetTextureHandle()
{
	return m_textureHandle;
}

bool Texture::IsCubeTexture()
{
	return m_isCubeTexture;
}

bool Texture::LoadTextureFromFile(char * filePath, TILING_MODE tilingMode)
{
	glGenTextures(1, &m_textureHandle);
	GLsizei iWidth, iHeight, bpp;
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	char *imageData = LoadTGA(filePath, &iWidth, &iHeight, &bpp);
	if (imageData == NULL) {
		return false;
	}
	m_width = iWidth;
	m_height = iHeight;

	GLenum imgFormat = (bpp == 24) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, iWidth, iHeight, 0, imgFormat, GL_UNSIGNED_BYTE, imageData);

	switch (tilingMode)
	{
	case TILING_MODE::CLAMP:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TILING_MODE::MIRROR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TILING_MODE::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] imageData;
	return true;
}

bool Texture::LoadCubeTextureFromFile(char * filePath, TILING_MODE tilingMode)
{
	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandle);

	GLsizei iWidth, iHeight, bpp;
	char *imgData;
	char *cubeData[6];
	bool isLoadImageSuccessful = true;
	imgData = LoadTGA(filePath, &iWidth, &iHeight, &bpp);
	if (imgData == NULL) {
		return false;
	}
	m_width = iWidth;
	m_height = iHeight;

	GLsizei iSubWidth, iSubHeight;
	iSubWidth = iWidth / 4;
	iSubHeight = iHeight / 3;
	int BytePerPixel = bpp / 8;
	for (int i = 0; i < 6; i++) {
		cubeData[i] = new char[iSubWidth*iSubHeight*BytePerPixel];
	}
	int soc = strlen(imgData);
	int iSubImgSize = (iSubWidth * iSubHeight) * BytePerPixel;
	char* pSubImgPlane[6] = {
		imgData + (4 * iSubWidth * iSubHeight + 2 * iSubWidth)*BytePerPixel,
		imgData + (4 * iSubWidth * iSubHeight + 0 * iSubWidth)*BytePerPixel,
		imgData + (0 * iSubWidth * iSubHeight + 1 * iSubWidth)*BytePerPixel,
		imgData + (8 * iSubWidth * iSubHeight + 1 * iSubWidth)*BytePerPixel,
		imgData + (4 * iSubWidth * iSubHeight + 1 * iSubWidth)*BytePerPixel,
		imgData + (4 * iSubWidth * iSubHeight + 3 * iSubWidth)*BytePerPixel,
	};

	for (int i = 0; i < iSubHeight; i++) {
		for (int iPlaneIndex = 0; iPlaneIndex < 6; iPlaneIndex++) {
			memcpy(cubeData[iPlaneIndex] + i * iSubWidth * BytePerPixel, pSubImgPlane[iPlaneIndex] + i * iWidth * BytePerPixel, iSubWidth * BytePerPixel);
		}
	}
	delete[] imgData;

	GLenum imgFormat = ((bpp == 24) ? GL_RGB : GL_RGBA);
	for (int i = 0; i<6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, imgFormat, iSubWidth, iSubHeight, 0, imgFormat, GL_UNSIGNED_BYTE, cubeData[i]);
	}

	switch (tilingMode)
	{
	case TILING_MODE::MIRROR:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TILING_MODE::REPEAT:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TILING_MODE::CLAMP:
	default:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	for (int i = 0; i < 6; i++) {
		delete[] cubeData[i];
	}
	return true;
}

bool Texture::LoadCubeTextureFromSplittedFiles(char ** imageFiles, TILING_MODE tilingMode)
{
	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandle);

	GLsizei iWidth, iHeight, bpp;
	char *cubeData[6];
	bool isLoadImageSuccessful = true;
	for (int i = 0; i < 6; i++) {
		cubeData[i] = LoadTGA(imageFiles[i], &iWidth, &iHeight, &bpp);
		if (cubeData[i] == NULL) isLoadImageSuccessful = false;
	}
	if (!isLoadImageSuccessful) {
		for (int i = 0; i < 6; i++) {
			delete[] cubeData[i];
		}
		return false;
	}
	m_width = iWidth;
	m_height = iHeight;
	GLenum imgFormat = (bpp == 24) ? GL_RGB : GL_RGBA;
	for (int i = 0; i<6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, imgFormat, iWidth, iHeight, 0, imgFormat, GL_UNSIGNED_BYTE, cubeData[i]);
	}

	switch (tilingMode)
	{
	case TILING_MODE::MIRROR:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TILING_MODE::REPEAT:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TILING_MODE::CLAMP:
	default:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	for (int i = 0; i < 6; i++) {
		delete[] cubeData[i];
	}
	return true;
}

float Texture::GetWidth() {
	return m_width;
}
float Texture::GetHeight() {
	return m_height;
}
