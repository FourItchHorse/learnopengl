#version 450 core
uniform vec3 inCol;
out vec4 outCol;
void main (void)
{
	outCol = vec4(inCol, 1.0);
}