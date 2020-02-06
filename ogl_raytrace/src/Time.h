#pragma once

#include "BlowTypes.h"
#include "GLFW/glfw3.h"

namespace tf
{
	struct Time
	{
		static f64 deltaTime;
		static f64 timeCurrentFrame;
		static f64 timePreviousFrame;

		// Not sure if needed
		static void init()
		{
			timePreviousFrame = glfwGetTime();
		}

		static void update()
		{
			timePreviousFrame = timeCurrentFrame;
			timeCurrentFrame = glfwGetTime();
			deltaTime = timeCurrentFrame - timePreviousFrame;
		}
	};
}