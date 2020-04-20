#version 450 core
out vec4 outCol;
in  flat uint ID;
void main () 
{
	vec4 myCol = vec4(0.0, 0.0, 0.0, 1.0);
	myCol[ID] += 0.75;
	outCol = myCol;
}
