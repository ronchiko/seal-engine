
#include <gl/glew.h>

#include <seal/gl.h>
#include <seal/io.h>
#include <seal/gl/texture.h>

#define SEAL_TEXTURE_RESERVED_LOADER	SEAL_NO_TEXTURE_INDEX

static Seal_TextureId gFallbackTexture = 0;
static int gFallbackTextureData[4] = {
	0xFFFF00FF, 0xFF202020,
	0xFF202020, 0xFFFF00FF,
};

static Seal_TextureId _LoadFallbackTexture(void) {
	if (gFallbackTexture <= 0) {
		GLCall(glEnable(GL_TEXTURE_2D));
		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		
		GLCall(glGenTextures(1, &gFallbackTexture));

		GLCall(glBindTexture(GL_TEXTURE_2D, gFallbackTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, gFallbackTextureData));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	}
	return gFallbackTexture;
} 


Seal_TextureId Seal_CreateTextureFromAsset(const char *path) {
	Seal_Image img = Seal_LoadImage(path);
	if(!img.buffer || img.width <= 0 || img.height <= 0) {
		Seal_LogError("Failed to load texture %s, Reverting to fallback", path);
		return _LoadFallbackTexture();
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
	if(texture != gFallbackTexture)
		glDeleteTextures(1, &texture);
}

