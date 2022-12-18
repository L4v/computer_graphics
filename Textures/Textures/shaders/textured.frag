#version 330 core

out vec4 FragColor;
in vec2 UVs;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform float uInterpolationFactor;

void main() {
	vec4 Texture1Color = texture(uTexture1, UVs);
	vec4 Texture2Color = texture(uTexture2, UVs);
	FragColor = mix(Texture1Color, Texture2Color, clamp(uInterpolationFactor, 0.0f, 1.0f));
}