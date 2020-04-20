#version 450 core 
out vec4 outCol;
in vec2 texCoord;
uniform sampler2D tex;
void main ()
{
	outCol = texture(tex, texCoord);
}