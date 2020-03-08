
#pragma once
#include <GL/gl3w.h>
#include <cstdlib>
#include <stdio.h>
#ifndef SHADER_H
#define SHADER_H
class ShaderReader
{
public:
	struct ShaderInfo
	{
		GLenum type;
		const char* filepath;
		GLuint shader;
	};
	static const char* ReadShader(const char* filepath);
	static GLuint LoadShaders(ShaderInfo* shaders);
};
#endif 
