#version 460 core
out vec4 FragColor;

uniform vec3 colSolid;

void main()
{
    FragColor = vec4(colSolid, 1.0f);
}