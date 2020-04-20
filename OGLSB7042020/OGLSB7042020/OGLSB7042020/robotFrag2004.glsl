#version 450 core
in VS_OUT 
{
vec4 outcol;
}fs_in;
out vec4 myCol;
void main() 
{
	myCol = fs_in.outcol;
}