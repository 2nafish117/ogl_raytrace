#pragma once

#include <iostream>

#include "stb_image/stb_image.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cassert>
#include "Time.h"

struct Texture
{
	unsigned int id;
	int width, height;

	void load(const char* path, GLenum mode = GL_CLAMP_TO_BORDER);
	void unload();
	void bind(unsigned int slot) const;
	void unbind() const;
};