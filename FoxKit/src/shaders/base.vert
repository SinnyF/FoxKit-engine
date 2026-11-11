#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat3 normalM;
uniform mat4 view;
uniform mat4 projection;

out vec3 fPos;
out vec3 fWorldPos;
out vec3 fNormal;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fPos = aPos;
    fWorldPos = vec3(model * vec4(aPos, 1.0));
    fNormal = normalM * aNormal;
}
