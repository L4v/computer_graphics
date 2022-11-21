#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
uniform vec3 offset;
out vec3 colorChannel;

void main() {
    colorChannel = aCol;
    gl_Position = vec4(aPos + offset, 1.0);
}
