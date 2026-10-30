
#version 460 core
out vec4 FragColor;
in vec3 outCol;

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;

void main()
{
    float high = max(max(outCol.r, outCol.g), outCol.b);
    high -= 0.05f;
    vec3 newCol = vec3(1f, 1f, 1f);
    newCol.r = max(outCol.r-high, 0f);
    newCol.g = max(outCol.g-high, 0f);
    newCol.b = max(outCol.b-high, 0f);
    newCol = normalize(newCol);

    vec3 finalColor = 
    newCol.r * color1 + 
    newCol.g * color2 +
    newCol.b * color3;

    finalColor = normalize(finalColor);

    FragColor = vec4(finalColor, 1.0f);
}
