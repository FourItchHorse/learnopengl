#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in float myID;
layout (location = 2) uniform float time;
layout (location = 3) uniform float angle;
out uint ID;
float rotation= 0.0f;
mat4 zRot(float angle)
{
	mat4 myRot = mat4(1.0f);
	myRot[0] = vec4(cos(angle), -sin(angle), 0.0, 0.0);
	myRot[1] = vec4(sin(angle), cos(angle), 0.0, 0.0);
	return myRot;
}
float determinePosition(float reference) 
{
	float interval = 1000;
	for(int i = 1; i < myID; i++)
	{
		interval *= 60;
	}
	if(uint(time)%uint(interval) == 0)
	{
		reference += angle;
	}
    return reference;
}
void main (void)
{
	ID = uint(myID) - 1;
	rotation = determinePosition(rotation);
	gl_Position = vec4(position, 1.0) * zRot(rotation);
}