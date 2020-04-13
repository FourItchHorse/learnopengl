#version 450 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 translation;
layout(location = 2) in vec4 axisAngle;
mat4 rotation;

mat4 ttranslate(vec3 t)
{
	mat4 transMat = mat4(1.0f);
	transMat[3] = vec4(t, 1.0f);
	return transMat;
}
mat4 qtransform(vec4 q)
{
	mat3 crossMat = mat3(1.0);
	for(int i = 0; i < crossMat.length(); i++)
	{
		crossMat[i] = cross(q.xyz, crossMat[i]);
	}
	mat3 rot = ((cos(q.w)) * mat3(1.0) + (sin(q.w)) * crossMat + (1-cos(q.w)) * outerProduct(q.xyz, q.xyz));
	return mat4(rot);
}
void main (void)
{
	gl_Position = vec4(position, 0.5, 1.0)  * qtransform(axisAngle) + vec4(translation, 1.0);
}