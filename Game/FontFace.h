#pragma once
#include "../Framework3D/Utilities/utilities.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <map>

class FontFace {
public:
	struct Character {
		GLuint uiTextureHandle;  // ID handle of the glyph texture
		int iWidth, iHeight;
		int iBearingX, iBearingY;    // Offset from baseline to left/top of glyph
		unsigned int uiAdvance;    // Offset to advance to next glyph
	};
private:
	int m_id;
	FT_Face m_font;
	int m_iMaxY, m_iMinY;
	std::map<char, Character> m_mapCharacters;
public:
	FontFace(int id);
	~FontFace();

	bool LoadFont(FT_Library library, char* filePath, int iPixelWidth, int iPixelHeight, bool isEnableAntiAlias = true);

	int GetId();
	int GetMaxY();
	int GetMinY();
	Character GetCharacter(unsigned char character);
};