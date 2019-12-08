#version 330 core
uniform vec3 Color;
out vec4 outColor;
void main()
{
   outColor = vec4(Color,1.0);
}