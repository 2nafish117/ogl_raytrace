#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include "BlowTypes.h"

namespace tf
{
	struct QuatFpsCamera
	{
		glm::quat orientation;
		glm::vec3 position;
		f32 near, far, fov, aspect;
		f32 speed, sensitivity;

		void init()
		{
			near = 0.1f;
			far = 100.0f;
			fov = glm::radians(60.0f);
			position = glm::vec3(0, 0, 5);
			orientation = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));
			sensitivity = 0.1f;
		}

		glm::mat4 getViewProj()
		{
			glm::mat4 rotMat = glm::toMat4(orientation);
			glm::mat4 transMat = glm::translate(glm::mat4(1.0f), -position);

			glm::mat4 view = rotMat * transMat;
			glm::mat4 proj = glm::perspective(fov, aspect, near, far);

			return proj * view;
		}

		void handleMouseMove(double xpos_prev, double ypos_prev, double xpos_now, double ypos_now)
		{
			double dx = xpos_now - xpos_prev;
			double dy = ypos_now - ypos_prev;

			// dx > 0 => rotate about -ve y axis (yaw)
			// dy > 0 => rotate about -ve x axis (pitch)

			dx = dx * sensitivity;
			dy = dy * sensitivity;

			glm::quat rot = glm::quat(glm::vec3(dy, dx, 0));
			// glm::quat rot = glm::toQuat(glm::vec3(dy, dx, 0));
			//glm::quat rot = glm::angleAxis((f32)dy, glm::vec3(1, 0, 0)) * glm::angleAxis((f32)-dx, glm::vec3(0, 1, 0));;
			rot = normalize(rot);

			orientation = rot;
			orientation = glm::normalize(orientation);
		}

	};

	struct EulerFpsCamera
	{
		glm::vec3 position;
		glm::vec3 front, up, right;
		glm::vec3 worldUp;
		f32 yaw, pitch, roll;
		f32 speed, sensitivity;
		f32 near, far, fov, aspect;

		void init()
		{
			position = glm::vec3(0, 0, 5);
			yaw = -glm::half_pi<float>() * 0;
			pitch = roll = 0;
			front = glm::vec3(0, 0, 1);
			worldUp = glm::vec3(0, 1, 0);

			right = glm::normalize(glm::cross(front, worldUp));
			up = glm::normalize(glm::cross(right, front));

			near = 0.1f;
			far = 100.0f;
			fov = glm::radians(60.0f);
			sensitivity = 0.01f;
		}

		glm::mat4 getViewProj()
		{
			glm::mat4 view = glm::lookAt(position, position + front, up);

			glm::mat4 projection = glm::perspective(fov, aspect, near, far);
			return projection * view;
		}

		void handleMouseMove(double xpos_prev, double ypos_prev, double xpos_now, double ypos_now)
		{
			double dx = xpos_now - xpos_prev;
			double dy = ypos_now - ypos_prev;
			std::cout << dx << ", " << dy << "\n";

			yaw += sensitivity * dx;
			pitch += sensitivity * dy;

			front.x = cos(yaw) * cos(pitch);
			front.y = sin(pitch);
			front.z = sin(yaw) * cos(pitch);
			front = glm::normalize(front);

			right = glm::normalize(glm::cross(front, worldUp));
			up = glm::normalize(glm::cross(right, front));
		}
	};
}