#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform vec3 offset;
uniform float scaling;
uniform vec3 flip;

out vec3 outCol;

void main()
{
    vec3 translatedPosition = ((aPos * flip) *scaling) + offset;
    gl_Position = vec4(translatedPosition, 1.0);
    outCol = aCol;
}
