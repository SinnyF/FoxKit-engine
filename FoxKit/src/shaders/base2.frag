#version 460 core
out vec4 FragColor;

in vec3 Pos;

void main()
{
    FragColor = vec4(Pos + 0.5f, 1.0f);
}