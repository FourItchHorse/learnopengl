#version 330 core
in vec3 Colour;
out vec4 outColor;
void main() 
{
	outColor = vec4(Colour, 1.0f);
}
