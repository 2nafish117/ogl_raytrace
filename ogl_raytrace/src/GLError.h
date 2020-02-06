#pragma once

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tf
{
	void GLAPIENTRY
		OnGlError(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam);
}