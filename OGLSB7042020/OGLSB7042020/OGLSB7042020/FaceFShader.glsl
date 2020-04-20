#version 450 core 
out vec4 outCol;
in vec2 texCoord;
uniform sampler2D tex;
void main ()
{
	vec4 myCol = vec4(1.0, 1.0, 1.0, 1.0);
	outCol = myCol * texture(tex, texCoord);
}