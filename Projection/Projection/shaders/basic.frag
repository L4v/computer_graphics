#version 330 core

out vec4 FragColour;

in vertex_out {
	vec3 FragColour;
} FragmentIn;

void main() {
	FragColour = vec4(1.0f);
}