#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 ambientLight;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 color;
uniform float specularPower = 256;
uniform float specularStrength = 1;
uniform sampler2D diffTexture;

in vec3 fPos;
in vec3 fWorldPos;
in vec3 fNormal;
in vec2 UV;

void main()
{
    FragColor = texture2D(diffTexture, UV);
    vec3 lightDir = normalize(lightPos - fWorldPos);
    vec3 diffuse = max(dot(fNormal,lightDir), 0.0) * lightColor;
    vec3 viewDir = normalize(viewPos - fWorldPos);
    vec3 reflectionDir = reflect(-lightDir, fNormal);
    vec3 specular = specularStrength * lightColor * pow(max(dot(viewDir, reflectionDir),0.0),specularPower);
    FragColor = FragColor * vec4(diffuse + ambientLight + specular, 1.0);
}