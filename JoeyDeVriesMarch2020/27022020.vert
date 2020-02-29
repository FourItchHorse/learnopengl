#version 450 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColour;
out vec3 Colour;
void main () 
{
	Colour = vColour;
	gl_Position = vec4(vPosition, 0.0f, 1.0f);
}
