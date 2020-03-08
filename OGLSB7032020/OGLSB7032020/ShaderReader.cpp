
#include "ShaderReader.h"
const char* ShaderReader::ReadShader(const char* filepath) 
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(filepath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		fprintf(stderr, "ERROR::SHADER_FILE::NOT_SUCCESSFULLY_READ\n");
	}
	return shaderCode.c_str();
}
 GLuint ShaderReader::LoadShaders(ShaderInfo* shaders) 
{
	GLuint program = glCreateProgram();
	while (shaders->type != GL_NONE)
	{
		GLuint shader = glCreateShader(shaders->type);
		shaders->shader = shader;
		const char* shaderSrc = ReadShader(shaders->filepath);
		glShaderSource(shader, 1, &shaderSrc, NULL);
		glCompileShader(shader);
		GLint compiled; glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if(!compiled)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			fprintf(stderr, "\n%s\n", infoLog);
		}
		glAttachShader(program, shader);
		++shaders;
	}
	glLinkProgram(program);
	GLint linked;  glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "\n%s\n", infoLog);
	}
	for (shaders; shaders->type != GL_NONE; ++shaders)
	{
		glDeleteShader(shaders->shader);
	}
	return program;
}

