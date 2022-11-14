#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 color;
uniform vec3 offset;
out vec4 outColor;

void main()
{
	outColor = color;
	gl_Position = vec4(inPos + offset, 1.0);
}