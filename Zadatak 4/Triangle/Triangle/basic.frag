#version 330 core

out vec4 FragColor;
in vec3 colorChannel;

void main() {
    FragColor = vec4(colorChannel, 1.0);
}