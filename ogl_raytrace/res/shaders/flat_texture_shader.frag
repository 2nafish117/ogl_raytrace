#version 430 core

// Input
in vec2 texCoord;

// Output
out vec4 oColor;

// Uniforms
layout(location = 5) uniform sampler2D uTexture;
layout(location = 6) uniform vec4 uTint = vec4(1, 1, 1, 1);

void main()
{
	oColor = texture(uTexture, texCoord) * uTint;
}