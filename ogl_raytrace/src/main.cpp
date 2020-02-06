#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLError.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Time.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

void OnFrameResize(GLFWwindow* window, int width, int height);

using namespace tf;

// Geometry data
#include "geometry/cube_indexed_textured_normals.inc"

// model loading sonar systems assimp
// https://www.youtube.com/watch?v=yQx_pMsYqzU

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		std::cout << "fps: " << 1 / Time::deltaTime << "\n";
}

int main(void)
{
	// Global Data
	GLFWwindow* window;
	int windowWidth  = 1024;
	int windowHeight = 1024;
	const char* windowText = "Raytrace";

	// Startup
	{
		if (!glfwInit())
			return -1;

		// Legacy OpenGl Doesnt Work with this enabled
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(windowWidth, windowHeight, windowText, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "[Error] Failed to initialize GLAD\n";
			glfwTerminate();
			return -1;
		}

		std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << "\n";
		std::cout << "OpenGL Vendor	: " << glGetString(GL_VENDOR) << "\n";
		std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << "\n";
		std::cout << "OpenGL Shader Language Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

		GLFWframebuffersizefun(frameBufferResizeHandler);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(tf::OnGlError, nullptr);
	}

	{
		// Local Data
		float verts[] = {
			// position		// texCoord				// normal
			 1.0f, -1.0f,	0.0f,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,	0.0f,	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,	0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			-1.0f, -1.0f,	0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		};

		u32 inds[] = {
			0, 1, 2,
			0, 2, 3
		};

		AOSMesh mesh;
		mesh.loadRawData((Vertex*) verts, 4, inds, 6);
		bool render = true;

		ShaderProgram shader;
		shader.load("res/shaders/raytrace.vert", "res/shaders/raytrace.frag");
		shader.bind();
		// only set initially, not updated on window resize
		shader.set1f("uWindowWidth", (f32) windowWidth);
		shader.set1f("uWindowHeight", (f32) windowHeight);

		// Initialization
		glfwSetFramebufferSizeCallback(window, OnFrameResize);
		glfwSetCursorPos(window, 0.0, 0.0);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			Time::update();

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);

			shader.set1f("uTime.now", Time::timeCurrentFrame);
			// shader.set1f("uTime.dt", Time::deltaTime);
			mesh.draw();
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		mesh.unload();
		shader.unload();

		glfwTerminate();
	}

	return 0;
}

void OnFrameResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
