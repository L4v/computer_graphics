#version 330 core

out vec4 FragColor;
in vec3 chCol;
uniform vec3 uCol;


void main() {
	FragColor = vec4(chCol + uCol, 1.0f);
}