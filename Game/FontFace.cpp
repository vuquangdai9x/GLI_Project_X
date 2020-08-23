#include "FontFace.h"

FontFace::FontFace(int id): m_id(id), m_iMinY(0), m_iMaxY(0), m_font(NULL)
{}

FontFace::~FontFace()
{
}

int FontFace::GetId()
{
	return m_id;
}

FontFace::Character FontFace::GetCharacter(unsigned char character)
{
	return m_mapCharacters[character];
}

int FontFace::GetMaxY() { return m_iMaxY; }
int FontFace::GetMinY() { return m_iMinY; }

bool FontFace::LoadFont(FT_Library library, char* filePath, int iPixelWidth, int iPixelHeight, bool isEnableAntiAlias) {
	FT_Face face;
	int iFT_Error;
	iFT_Error = FT_New_Face(library, filePath, 0, &face);
	if (iFT_Error == FT_Err_Unknown_File_Format)
	{
		// the font file could be openedand read, but it appears that its font format is unsupported
		printf("[ERR] FontFace: Failed to load Font. Font format is not support\n");
		return false;
	}
	else if (iFT_Error)
	{
		// another error code means that the font file could not be opened or read, or that it is broken
		printf("[ERR] FontFace: Failed to load Font: Cannot open file\n");
		return false;
	}
	iFT_Error = FT_Set_Pixel_Sizes(face, iPixelWidth, iPixelHeight);
	if (iFT_Error) {
		printf("[ERR] FontFace: Failed to set pixel size\n");
		return false;
	}
	/*FT_Matrix ftMat;
	ftMat.xx = (FT_Fixed)(1 * 0x10000L);
	ftMat.xy = (FT_Fixed)(0 * 0x10000L);
	ftMat.yx = (FT_Fixed)(0 * 0x10000L);
	ftMat.yy = (FT_Fixed)(-1 * 0x10000L);
	FT_Set_Transform(face, &ftMat, NULL);*/

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	m_iMaxY = m_iMinY = 0;
	for (unsigned char c = 0; c < 128; c++)
	{
		iFT_Error = FT_Load_Char(face, c, (isEnableAntiAlias ? FT_LOAD_RENDER : FT_LOAD_MONOCHROME));
		// load character glyph 
		if (iFT_Error)
		{
			printf("[Wrn] FontFace: Failed to load glyph : %u (ascii-code) : %c (character)\n",c,c);
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			face->glyph->bitmap_left, face->glyph->bitmap_top,
			face->glyph->advance.x >> 6
		};
		m_mapCharacters.insert(std::pair<char, Character>(c, character));

		if (m_iMaxY < character.iBearingY) 
			m_iMaxY = face->glyph->bitmap_top;
		if (m_iMinY > character.iBearingY - character.iHeight)
			m_iMinY = character.iBearingY - character.iHeight;
	}
	FT_Done_Face(face);
	return true;
}