#version 430 core
// Inputs
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

// Outputs
out vec2 texCoord;

// Uniforms
layout (location = 3) uniform mat4 uTransform;
layout (location = 4) uniform mat4 uViewProjection;

void main()
{
	texCoord = aTexCoord;
	vec4 transformedPos = uViewProjection * uTransform * vec4(aPosition, 1.0f);
	// gl_Position = vec4(aPosition, 1.0f);
	gl_Position = transformedPos;
}