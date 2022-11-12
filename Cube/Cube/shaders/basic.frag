#version 330 core

out vec4 FragColor;

in vertex_output {
    vec3 FragColor;
} FragmentInput;

void main() {
    FragColor = vec4(FragmentInput.FragColor, 1.0);
}