#version 330 core

in vec2 texCoord;

out vec4 oColor;

uniform sampler2D uTexture;
uniform vec4 uTint = vec4(1, 1, 1, 1);

void main()
{
	oColor = texture(uTexture, texCoord) * uTint;
}