#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vertex_output {
    vec3 FragColor;    
} VertexOutput;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    VertexOutput.FragColor = aCol;
}
