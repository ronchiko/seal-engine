
#include <gl/glew.h>

#include <seal/gl.h>
#include <seal/io.h>
#include <seal/gl/texture.h>

#define SEAL_TEXTURE_RESERVED_LOADER	SEAL_NO_TEXTURE_INDEX


Seal_TextureId Seal_CreateTextureFromAsset(const char *path) {
	Seal_Image img = Seal_LoadImage(path);
	if(!img.buffer || img.width <= 0 || img.height <= 0) {
		Seal_LogError("Failed to load texture %s", SEAL_FALSE, path);
		return 0;
	}

	GLCall(glEnable(GL_TEXTURE_2D));
	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	
	Seal_TextureId tid = 0;
	GLCall(glGenTextures(1, &tid));

	GLCall(glBindTexture(GL_TEXTURE_2D, tid));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.buffer));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	Seal_FreeImage(img);
	return tid;
}

void Seal_FreeTexture(Seal_TextureId texture) {
	glDeleteTextures(1, &texture);
}

