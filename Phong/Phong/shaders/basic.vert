#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 UV;
out vec3 vWorldSpaceFragment;
out vec3 vWorldSpaceNormal;

void main() {
	vWorldSpaceFragment = vec3(uModel * vec4(aPos, 1.0f));
	vWorldSpaceNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);

	UV = aUV;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}