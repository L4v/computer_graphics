#version 330 core

out vec4 FragColor;
uniform float offset;

void main() {
    FragColor = vec4(1.0 * offset, 0.5 * offset, 0.0, 1.0) ;
}