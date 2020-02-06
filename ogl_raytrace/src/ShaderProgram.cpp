#include "ShaderProgram.h"

namespace tf
{
	void ShaderProgram::load(const char* vsPath, const char* fsPath)
	{
		FILE* vertexFile = nullptr;
		fopen_s(&vertexFile, vsPath, "rb");
		FILE* fragmentFile = nullptr;
		fopen_s(&fragmentFile, fsPath, "rb");

		if (!vertexFile)
			std::cerr << "[ERROR] file '" << vsPath << "' doesn't exist\n";
		if (!fragmentFile)
			std::cerr << "[ERROR] file '" << fsPath << "' doesn't exist\n";
		if (!vertexFile || !fragmentFile)
			return;

		const char* vertexSource = readSource(vertexFile);
		const char* fragmentSource = readSource(fragmentFile);

		unsigned int vs = compileShader(vertexSource, GL_VERTEX_SHADER, vsPath);
		unsigned int fs = compileShader(fragmentSource, GL_FRAGMENT_SHADER, fsPath);

		linkProgram(vs, fs);

		if (vertexSource)
			delete[] vertexSource;
		if (fragmentSource)
			delete[] fragmentSource;

		glDeleteShader(vs);
		glDeleteShader(fs);
		fclose(vertexFile);
		fclose(fragmentFile);
	}

	void ShaderProgram::unload()
	{
		glDeleteProgram(id);
	}

	void ShaderProgram::set1i(const std::string & name, int value) const
	{
		int location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void ShaderProgram::set1f(const std::string & name, float value) const
	{
		int location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void ShaderProgram::set3f(const std::string & name, float f0, float f1, float f2) const
	{
		int location = getUniformLocation(name);
		glUniform3f(location, f0, f1, f2);
	}

	void ShaderProgram::set4f(const std::string & name, float f0, float f1, float f2, float f3) const
	{
		int location = getUniformLocation(name);
		glUniform4f(location, f0, f1, f2, f3);
	}

	void ShaderProgram::setMat4fv(const std::string & name, const float * data) const
	{
		int location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, data);
	}

	const char * ShaderProgram::readSource(FILE* file) const
	{
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);

		char* source = new char[size + 1];

		rewind(file);
		fread(source, sizeof(char), size, file);
		source[size] = '\0';

		return source;
	}

	unsigned int ShaderProgram::compileShader(const char * src, GLenum type, const char* fpath) const
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (compiled == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* buff = (char*)alloca(length + 1);
			glGetShaderInfoLog(shader, length, &length, buff);

			std::cerr << "[ERROR] : " << fpath << ": " << buff << "\n";
		}

		return shader;
	}

	void ShaderProgram::linkProgram(unsigned int vs, unsigned int fs)
	{
		id = glCreateProgram();
		glAttachShader(id, vs);
		glAttachShader(id, fs);
		glLinkProgram(id);

		int link;
		glGetProgramiv(id, GL_LINK_STATUS, &link);

		if (link == GL_FALSE)
		{
			int length;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char* buff = (char*)alloca(length + 1);
			glGetProgramInfoLog(id, length, &length, buff);
			std::cerr << "[ERROR] : " << buff << "\n";
		}
	}

	int ShaderProgram::getUniformLocation(const std::string & name) const
	{
		int location = glGetUniformLocation(id, name.c_str());

		if (location == -1)
			std::cerr << "[ERROR] uniform " << name << " doesn't exist\n";

		return location;
	}

	int ShaderProgram::getUniformLocation(const char* name) const
	{
		int location = glGetUniformLocation(id, name);

		if (location == -1)
			std::cerr << "[ERROR] uniform " << name << " doesn't exist\n";

		return location;
	}
}