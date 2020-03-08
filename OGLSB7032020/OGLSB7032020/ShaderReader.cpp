
#include "ShaderReader.h"
const char* ShaderReader::ReadShader(const char* filepath) 
{
	FILE* infile = fopen(filepath, "rb");
	if (!infile)
	{
		fprintf( stderr, "Unable to open file '%s,'\n", filepath);
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	GLchar* source = new GLchar[len];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;
	printf(source);
	return const_cast<const GLchar*>(source);
}
 GLuint ShaderReader::LoadShaders(ShaderInfo* myShaders) 
{
	if (myShaders == NULL) return 0;
	ShaderInfo* shaders = myShaders;
	GLuint program = glCreateProgram();
	while (shaders->type != GL_NONE)
	{
		GLuint shader = glCreateShader(shaders->type);
		shaders->shader = shader;
		if (shaders->filepath != NULL) {
			const char* shaderSrc = ReadShader(shaders->filepath);
			glShaderSource(shader, 1, &shaderSrc, NULL);
		}
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

