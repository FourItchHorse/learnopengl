#pragma once
#include <cstdlib>
#include <stdio.h>
#include <GL/gl3w.h>
extern "C" {
	typedef struct {
		GLenum type;
		const char* filename;
		GLuint shader;
	} ShaderInfo;

	static const GLchar* ReadShader(const char* filename) 
	{
		FILE* infile = fopen(filename, "rb");
		if (!infile)
		{
			fprintf(stderr, "Unable to open file %s\n", filename);
			return NULL;
		}
		fseek(infile, 0, SEEK_END);
		int len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		GLchar* source = new GLchar[len+1];
		fread(source, 1, len, infile);
		fclose(infile);
		source[len] = 0;
		return const_cast<const GLchar*>(source);
	}
	GLuint LoadShaders(ShaderInfo* shaders)
	{
		if (shaders == NULL)return 0;
		GLuint program = glCreateProgram();
		ShaderInfo* entry = shaders;
		while (entry->type != GL_NONE) 
		{
			GLuint shader = glCreateShader(entry->type);
			entry->shader = shader;
			const GLchar* source = ReadShader(entry->filename);
			if (source == NULL) {
				for (entry = shaders; entry->type != GL_NONE; ++entry)
					glDeleteShader(entry->shader);
				return 0;
			}
			glShaderSource(shader, 1, &source, NULL);
			delete[] source;
			glCompileShader(shader);
			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) 
			{
				GLsizei len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
				GLchar* log = new GLchar[len + 1];
				glGetShaderInfoLog(shader, len, &len, log);
				fprintf(stderr, "COMPILATION OF %s FAILED:\n%s\n", entry->filename, log);
				return 0;
			}
			glAttachShader(program, shader);
			glDeleteShader(shader);
			++entry;
		}
		glLinkProgram(program);
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			fprintf(stderr, "FAILED TO LINK PROGRAM:\n%s\n", log);
			for (entry = shaders; entry->type != GL_NONE; ++entry)
				glDeleteShader(entry->shader);
			return 0;
		}
		return program;
	}
};