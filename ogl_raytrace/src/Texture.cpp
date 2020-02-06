#include "Texture.h"

void Texture::load(const char * path, GLenum mode)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (!data)
	{
		std::cerr << "[ERROR] file " << path << " doesn't exist\n";
		width = height = 0;
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture::unload()
{
	assert(id > 0);
	glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
