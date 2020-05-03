#version 450 core
layout (location = 0) in vec4 inPos;
layout(location = 1) uniform mat4 mvp;
void main (void)
{
	gl_Position =  vec4(inPos);
}