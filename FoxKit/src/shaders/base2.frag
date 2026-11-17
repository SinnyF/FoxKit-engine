#version 460 core
out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 ambientLight;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 color;
uniform float specularPower = 256;
uniform float specularStrength = 1.0;
uniform float tiling = 1.0;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform bool textureOn;
uniform bool normalOn;

in vec3 fPos;
in vec3 fWorldPos;
in vec3 fNormal;
in vec2 UV;
in mat3 TBN;

void main(){
    vec3 textureNormal;
    if(textureOn)
        FragColor = texture2D(diffuseMap, UV*tiling);
    else
        FragColor = vec4(color, 1.0);
    if(normalOn)
    {
        textureNormal = texture2D(normalMap, UV*tiling).rgb;
        textureNormal = textureNormal * 2.0 - 1.0;
        textureNormal = normalize(TBN * textureNormal);
    }
    else
        textureNormal = fNormal;
    
    vec3 diffuse = max(dot(textureNormal,lightDir), 0.0) * lightColor;
    vec3 viewDir = normalize(viewPos - fWorldPos);
    vec3 reflectionDir = reflect(-lightDir, textureNormal);
    vec3 specular = specularStrength * lightColor * pow(max(dot(viewDir, reflectionDir),0.0),specularPower);
    FragColor = FragColor * vec4(diffuse + ambientLight + specular, 1.0);
}