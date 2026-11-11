#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 ambientLight;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 color;

in vec3 fPos;
in vec3 fWorldPos;
in vec3 fNormal;

void main()
{
    FragColor = vec4(color, 1.0f);
    vec3 lightDir = normalize(lightPos - fWorldPos);
    vec3 diffuse = max(dot(fNormal,lightDir), 0.0) * lightColor;
    vec3 viewDir = normalize(viewPos - fWorldPos);
    vec3 reflectionDir = reflect(-lightDir, fNormal);
    vec3 specular = 1.0 * lightColor * pow(max(dot(viewDir, reflectionDir),0.0),256);
    FragColor = FragColor * vec4(diffuse + ambientLight + specular, 1.0);
}