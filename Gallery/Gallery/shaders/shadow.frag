#version 330 core

in vec4 FragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

void main() {
    float LightDistance = length(FragPos.xyz - uLightPos);
    LightDistance = LightDistance / uFarPlane;
    gl_FragDepth = LightDistance;
}