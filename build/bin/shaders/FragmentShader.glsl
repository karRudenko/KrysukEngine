#version 330 core
out vec4 FragColor;
in vec3 fragmentColor;
out vec3 color;
in vec2 UV;
uniform sampler2D myTextureSampler;
void main()
{
    color = texture( myTextureSampler, UV ).rgb;
} 