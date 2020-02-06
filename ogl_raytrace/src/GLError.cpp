#include "GLError.h"

namespace tf
{
	struct MessageCallbackParam
	{
		const char* fname;
		unsigned int line;
		const char* funcSig;
	};

	void GLAPIENTRY
		OnGlError(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		//MessageCallbackParam* param = (MessageCallbackParam*)userParam;
		
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);

		//if (param != nullptr)
		//{
		//	std::cerr << "[" << severity << " GL ERROR " << std::dec << type << std::hex << " ] " << param->fname << ": "
		//		<< param->line << ": " << param->funcSig << "\n";
		//	std::cerr << "\t" << message << "\n";
		//
		//}
		//else
		//{
		//	std::cerr << "[" << severity << " GL ERROR " << std::dec << type << std::hex << " ] " << "\n";
		//	std::cerr << "\t" << message << "\n";
		//}
		if(type == GL_DEBUG_TYPE_ERROR)
			__debugbreak();
	}
}