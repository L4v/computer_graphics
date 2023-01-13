#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

struct PositionalLight {
	vec3 Position;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Kc;
	float Kl;
	float Kq;
};

struct DirectionalLight {
	vec3 Position;
	vec3 Direction;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float InnerCutOff;
	float OuterCutOff;
	float Kc;
	float Kl;
	float Kq;
};

uniform PositionalLight uPointLight;
uniform DirectionalLight uSpotlight;
uniform DirectionalLight uDirLight;
uniform vec3 uViewPos;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 vCol;

void main() {
	vec3 WorldSpaceVertex = vec3(uModel * vec4(aPos, 1.0f));
	vec3 WorldSpaceNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	vec3 ViewDirection = normalize(uViewPos - WorldSpaceVertex);

	// NOTE(Jovan): Directional light
	vec3 DirLightVector = normalize(-uDirLight.Direction);
	float DirDiffuse = max(dot(WorldSpaceNormal, DirLightVector), 0.0f);
	vec3 DirReflectDirection = reflect(-DirLightVector, WorldSpaceNormal);
	// NOTE(Jovan): 32 is the specular shininess factor. Hardcoded for now
	float DirSpecular = pow(max(dot(ViewDirection, DirReflectDirection), 0.0f), 32.0f);

	vec3 DirAmbientColor = uDirLight.Ka;
	vec3 DirDiffuseColor = uDirLight.Kd * DirDiffuse;
	vec3 DirSpecularColor = uDirLight.Ks * DirSpecular;
	vec3 DirColor = DirAmbientColor + DirDiffuseColor + DirSpecularColor;

	// NOTE(Jovan): Point light
	vec3 PtLightVector = normalize(uPointLight.Position - WorldSpaceVertex);
	float PtDiffuse = max(dot(WorldSpaceNormal, PtLightVector), 0.0f);
	vec3 PtReflectDirection = reflect(-PtLightVector, WorldSpaceNormal);
	float PtSpecular = pow(max(dot(ViewDirection, PtReflectDirection), 0.0f), 32.0f);

	vec3 PtAmbientColor = uPointLight.Ka;
	vec3 PtDiffuseColor = PtDiffuse * uPointLight.Kd;
	vec3 PtSpecularColor = PtSpecular * uPointLight.Ks;

	float PtLightDistance = length(uPointLight.Position - WorldSpaceVertex);
	float PtAttenuation = 1.0f / (uPointLight.Kc + uPointLight.Kl * PtLightDistance + uPointLight.Kq * (PtLightDistance * PtLightDistance));
	vec3 PtColor = PtAttenuation * (PtAmbientColor + PtDiffuseColor + PtSpecularColor);

	// NOTE(Jovan): Spotlight
	vec3 SpotlightVector = normalize(uSpotlight.Position - WorldSpaceVertex);

	float SpotDiffuse = max(dot(WorldSpaceNormal, SpotlightVector), 0.0f);
	vec3 SpotReflectDirection = reflect(-SpotlightVector, WorldSpaceNormal);
	float SpotSpecular = pow(max(dot(ViewDirection, SpotReflectDirection), 0.0f), 32.0f);

	vec3 SpotAmbientColor = uSpotlight.Ka;
	vec3 SpotDiffuseColor = SpotDiffuse * uSpotlight.Kd;
	vec3 SpotSpecularColor = SpotSpecular * uSpotlight.Ks;

	float SpotlightDistance = length(uSpotlight.Position - WorldSpaceVertex);
	float SpotAttenuation = 1.0f / (uSpotlight.Kc + uSpotlight.Kl * SpotlightDistance + uSpotlight.Kq * (SpotlightDistance * SpotlightDistance));

	float Theta = dot(SpotlightVector, normalize(-uSpotlight.Direction));
	float Epsilon = uSpotlight.InnerCutOff - uSpotlight.OuterCutOff;
	float SpotIntensity = clamp((Theta - uSpotlight.OuterCutOff) / Epsilon, 0.0f, 1.0f);
	vec3 SpotColor = SpotIntensity * SpotAttenuation * (SpotAmbientColor + SpotDiffuseColor + SpotSpecularColor);

	vCol = DirColor + PtColor + SpotColor;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}
