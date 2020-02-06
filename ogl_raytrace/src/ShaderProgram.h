#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>
#include <string>
#include "Time.h"

namespace tf
{
	struct ShaderProgram
	{
		unsigned int id;

		void load(const char* vsPath, const char* fsPath);
		void unload();
		void bind() const { glUseProgram(id); }
		void unbind() const { glUseProgram(0); }

		void set1i(const std::string& name, int value) const;

		void set1f(const std::string& name, float value) const;
		void set3f(const std::string& name, float f0, float f1, float f2) const;
		void set4f(const std::string& name, float f0, float f1, float f2, float f3) const;

		void setMat4fv(const std::string& name, const float* data) const;

	private:
		const char* readSource(FILE* file) const;
		unsigned int compileShader(const char* src, GLenum type, const char* fpath) const;
		void linkProgram(unsigned int vs, unsigned int fs);
		int getUniformLocation(const std::string& name) const;
		int getUniformLocation(const char* name) const;
	};
}
