#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 uShadowMatrices[6];

out vec4 FragPos;

void main() {
    for(int FaceIdx = 0; FaceIdx < 6; ++FaceIdx) {
        gl_Layer = FaceIdx;
        for(int i = 0; i < 3; ++i) {
            FragPos = gl_in[i].gl_Position;
            gl_Position = uShadowMatrices[FaceIdx] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}