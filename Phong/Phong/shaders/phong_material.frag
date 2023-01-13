#version 330 core

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

struct Material {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform PositionalLight uPointLight;
uniform DirectionalLight uSpotlight;
uniform DirectionalLight uDirLight;
uniform Material uMaterial;
uniform vec3 uViewPos;

in vec3 vWorldSpaceFragment;
in vec3 vWorldSpaceNormal;

out vec4 FragColor;

void main() {
	vec3 ViewDirection = normalize(uViewPos - vWorldSpaceFragment);
	// NOTE(Jovan): Directional light
	vec3 DirLightVector = normalize(-uDirLight.Direction);
	float DirDiffuse = max(dot(vWorldSpaceNormal, DirLightVector), 0.0f);
	vec3 DirReflectDirection = reflect(-DirLightVector, vWorldSpaceNormal);
	// NOTE(Jovan): 32 is the specular shininess factor. Hardcoded for now
	float DirSpecular = pow(max(dot(ViewDirection, DirReflectDirection), 0.0f), uMaterial.Shininess);

	vec3 DirAmbientColor = uDirLight.Ka * uMaterial.Ka;
	vec3 DirDiffuseColor = uDirLight.Kd * DirDiffuse * uMaterial.Kd;
	vec3 DirSpecularColor = uDirLight.Ks * DirSpecular * uMaterial.Ks;
	vec3 DirColor = DirAmbientColor + DirDiffuseColor + DirSpecularColor;

	// NOTE(Jovan): Point light
	vec3 PtLightVector = normalize(uPointLight.Position - vWorldSpaceFragment);
	float PtDiffuse = max(dot(vWorldSpaceNormal, PtLightVector), 0.0f);
	vec3 PtReflectDirection = reflect(-PtLightVector, vWorldSpaceNormal);
	float PtSpecular = pow(max(dot(ViewDirection, PtReflectDirection), 0.0f), uMaterial.Shininess);

	vec3 PtAmbientColor = uPointLight.Ka * uMaterial.Ka;
	vec3 PtDiffuseColor = PtDiffuse * uPointLight.Kd * uMaterial.Kd;
	vec3 PtSpecularColor = PtSpecular * uPointLight.Ks * uMaterial.Ks;

	float PtLightDistance = length(uPointLight.Position - vWorldSpaceFragment);
	float PtAttenuation = 1.0f / (uPointLight.Kc + uPointLight.Kl * PtLightDistance + uPointLight.Kq * (PtLightDistance * PtLightDistance));
	vec3 PtColor = PtAttenuation * (PtAmbientColor + PtDiffuseColor + PtSpecularColor);

	// NOTE(Jovan): Spotlight
	vec3 SpotlightVector = normalize(uSpotlight.Position - vWorldSpaceFragment);

	float SpotDiffuse = max(dot(vWorldSpaceNormal, SpotlightVector), 0.0f);
	vec3 SpotReflectDirection = reflect(-SpotlightVector, vWorldSpaceNormal);
	float SpotSpecular = pow(max(dot(ViewDirection, SpotReflectDirection), 0.0f), uMaterial.Shininess);

	vec3 SpotAmbientColor = uSpotlight.Ka * uMaterial.Ka;
	vec3 SpotDiffuseColor = SpotDiffuse * uSpotlight.Kd * uMaterial.Kd;
	vec3 SpotSpecularColor = SpotSpecular * uSpotlight.Ks * uMaterial.Ks;

	float SpotlightDistance = length(uSpotlight.Position - vWorldSpaceFragment);
	float SpotAttenuation = 1.0f / (uSpotlight.Kc + uSpotlight.Kl * SpotlightDistance + uSpotlight.Kq * (SpotlightDistance * SpotlightDistance));

	float Theta = dot(SpotlightVector, normalize(-uSpotlight.Direction));
	float Epsilon = uSpotlight.InnerCutOff - uSpotlight.OuterCutOff;
	float SpotIntensity = clamp((Theta - uSpotlight.OuterCutOff) / Epsilon, 0.0f, 1.0f);
	vec3 SpotColor = SpotIntensity * SpotAttenuation * (SpotAmbientColor + SpotDiffuseColor + SpotSpecularColor);
	
	vec3 FinalColor = DirColor + PtColor + SpotColor;
	FragColor = vec4(FinalColor, 1.0f);
}
