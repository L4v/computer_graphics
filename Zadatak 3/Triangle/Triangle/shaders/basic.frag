#version 330 core

#define MAX_POINT_LIGHT_COUNT 8

out vec4 FragColor;

struct PointLight {
    vec3 Position;

    float Kc;
    float Kl;
    float Kq;

    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform vec3        uViewPos;
uniform PointLight  uPointLights[MAX_POINT_LIGHT_COUNT];
uniform float       uTexScale;
uniform sampler2D   uDiffuse;
uniform sampler2D   uSpecular;
uniform samplerCube uDepthMap;
uniform int         uPointLightCount;
uniform float       uFarPlane;

vec2 ScaledTexCoord;

vec3
CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float
ShadowCalculation(vec3 fragPos);


void main() {
    ScaledTexCoord = uTexScale * (TexCoord + 0.5) + 0.5 * uTexScale;

    // TODO(Jovan): Optimize
    vec3 NormalizedNormal = normalize(Normal);
    vec3 ViewDir = normalize(uViewPos - FragPos);

    vec3 Result = vec3(0.0f);

    for(int PtLightIdx = 0; PtLightIdx < uPointLightCount; ++PtLightIdx) {
        Result += CalculatePointLight(uPointLights[PtLightIdx], NormalizedNormal, FragPos, ViewDir);
    }

    FragColor = vec4(Result, 1.0);
}

float
ShadowCalculation(vec3 fragPos, vec3 lightPos) {
    vec3 FragToLight = fragPos - lightPos;
    float ClosestDepth = texture(uDepthMap, FragToLight).r;
    ClosestDepth *= uFarPlane;
    float CurrentDepth = length(FragToLight);
    float Bias = 0.05f;
    float Shadow = CurrentDepth - Bias > ClosestDepth ? 1.0f : 0.0f;
    return Shadow;
}

vec3
CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 LightDir = normalize(light.Position - fragPos);
    float Diffuse = max(dot(normal, LightDir), 0.0);
    
    vec3 HalfwayDir = normalize(LightDir + viewDir);
    float Specular = pow(max(dot(normal, HalfwayDir), 0.0), 128.0f);

    float Distance = length(light.Position - FragPos);
    float Attenuation = 1.0 / (light.Kc + light.Kl * Distance + light.Kq * (Distance * Distance));
    
    vec3 vAmbient = light.Ka * vec3(texture(uDiffuse, ScaledTexCoord));
    vec3 vDiffuse = light.Kd * Diffuse * vec3(texture(uDiffuse, ScaledTexCoord));
    vec3 vSpecular = light.Ks * Specular * vec3(texture(uSpecular, ScaledTexCoord));

    //float Shadow = ShadowCalculation(FragPos, light.Position);

    //return (vAmbient + (1.0f - Shadow) * (vDiffuse + vSpecular)) * Attenuation;
    return (vAmbient + (vDiffuse + vSpecular)) * Attenuation;
}
