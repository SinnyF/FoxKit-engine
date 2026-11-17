#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat3 normalM;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPos;
out vec3 fWorldPos;
out vec3 fNormal;
out vec2 UV;
out mat3 TBN;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fPos = aPos;
    fWorldPos = vec3(model * vec4(aPos, 1.0));
    fNormal = normalize(normalM * aNormal);
    vec3 T = normalize(normalM * aTangent);
    vec3 B = normalize(normalM * aBitangent);
    vec3 N = normalize(normalM * aNormal);
    TBN = mat3(T, B, N);
    UV = aUV;
}
