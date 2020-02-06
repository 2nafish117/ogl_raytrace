#version 430 core
// Inputs
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;	// ignore
layout (location = 2) in vec3 aNormal;		// ignore

// Outputs

// Uniforms

void main()
{
	gl_Position = vec4(aPosition, 1.0);
}