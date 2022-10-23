#version 330 core
// NOTE(Jovan): A "procedural network" shader thing
#define S(x, y, t) smoothstep(x, y, t)

uniform vec2  uResolution;
uniform float uTime;

float
DistLine(vec2 p, vec2 x, vec2 y) {
    vec2 XP = p - x;
    vec2 XY = y - x;
    
    float t = clamp(dot(XP, XY) / dot(XY, XY), 0.0f, 1.0f);
    
    return length(XP - XY * t);
}

float RandV2F(vec2 p) {
    p = fract(p * vec2(233.4f, 851.73f));
    p += dot(p, p + 23.45f);
    return fract(p.x * p.y);
}

vec2 RandV2V2(vec2 p) {
    float N = RandV2F(p);
    return vec2(N, RandV2F(p + N));
}

// NOTE(L4v): Returns position inside cell with offset
vec2 GetPos(vec2 id, vec2 offset) {
    
    vec2 R = RandV2V2(id + offset) * uTime;
    return offset + vec2(cos(R.x), sin(R.y)) * 0.4f;
}

float Line(vec2 p, vec2 a, vec2 b) {
    float Dist = DistLine(p, a, b);
    float Len = length(b - a);
    float M = S(0.03f, 0.01f, Dist);
    M *= S(1.2f, 0.8f, Len) * 0.5f + S(0.05f, 0.03f, abs(Len - 0.75f));
    return M;
}

float Layer(vec2 uv) {
    float M = 0.0f;
    vec2 GV = fract(uv) - 0.5f;
    
    // NOTE(L4v): ID of each grid cell
    vec2 ID = floor(uv);
    vec2 P[9];
    
    int GridIdx = 0;
    // NOTE(L4v): Iterate through neighbours
    for(float Y = -1.0f; Y <= 1.0f; ++Y) {
        for(float X = -1.0f; X <= 1.0f; ++X) {
            P[GridIdx++] = GetPos(ID, vec2(X, Y));
        }
    }
    
    float T = uTime * 10.0f;
    for(int PtIdx = 0; PtIdx < 9; ++PtIdx) {
        M += Line(GV, P[4], P[PtIdx]);
        vec2 J = (P[PtIdx] - GV) * 20.0f;
        float Sparkle = 1.0f / dot(J, J);
        M += Sparkle * (sin(T + fract(P[PtIdx].x) * 10.0f) * 0.5f + 0.5f);
    }
    M += Line(GV, P[1], P[3]);
    M += Line(GV, P[1], P[5]);
    M += Line(GV, P[7], P[3]);
    M += Line(GV, P[7], P[5]);
    
    return M;
}

in  vec3 FragPos;
out vec4 FragColor;

void main()
{
    vec2 uv = (FragPos.xz - 0.5f * uResolution.xy) / uResolution.y;
    
    float M = 0.0f;
    float T = uTime * 0.1f;
    float LayerCount = 4.0f;
    float Sin = sin(T);
    float Cos = cos(T);
    
    mat2 Rot = mat2(Cos, -Sin, Sin, Cos);
    
    uv *= Rot;
    for(float LayerIdx = 0.0f; LayerIdx < 1.0f; LayerIdx += 1.0f / LayerCount) {
        float Z = fract(LayerIdx + T);
        float Size = mix(10.0f, 0.5f, Z);
        float Fade = S(0.0f, 0.5f, Z) * S(1.0f, 0.8f, Z);
        
        M += Layer(uv * Size + LayerIdx * 20.0f) * Fade;
    }
    
    float ColorShiftSpeed = 5.0f;
    vec3 BaseColor = sin(ColorShiftSpeed * T * vec3(0.345f, 0.456f, 0.657f))*0.4f + 0.6f;
    vec3 col = vec3(M * BaseColor);

    FragColor = vec4(col, 1.0);
}