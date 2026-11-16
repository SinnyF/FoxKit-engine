#version 460 core
out vec4 FragColor;

uniform vec3 color = vec3(1,0,0);
uniform float time;

void main()
{
    FragColor = vec4(mix(vec3(0.2,0,0),color,(sin(time * 6) + 1)/2),1);
}
