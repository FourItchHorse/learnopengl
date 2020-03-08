#version 450 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;
layout (location = 2) out vec3 Col;
void main (void)
{
	Col = col;
	gl_Position = vec4(pos, 0.5, 1.0);
}
