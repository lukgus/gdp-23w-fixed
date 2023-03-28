#include "Texture.h"

#include "GL.h"
#include <FreeImage/FreeImage.h>

namespace gdp
{
	static GLenum gEnum = GL_TEXTURE0;

	Texture::Texture(const char* filepath) {
		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filepath, 0), filepath);
		FIBITMAP* image = FreeImage_ConvertTo32Bits(bitmap);

		int width = FreeImage_GetWidth(image);
		int height = FreeImage_GetHeight(image);

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image));

		FreeImage_Unload(image);

		glEnum = gEnum++;
	}

	Texture::~Texture() {

	}
}