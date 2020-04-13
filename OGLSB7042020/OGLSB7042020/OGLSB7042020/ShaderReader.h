#pragma once
#include <cstdlib>
#include <stdio.h>
#include <GL/GL.h>
extern "C" {
	typedef struct {
		GLenum type;
		const char* filename;
		GLuint shader;
	}ShaderInfo;

	static const GLchar* 
	ReadShader(const char* filename)
	{
		FILE* infile = fopen(filename, "rb");
		if (!infile) 
		{
#ifdef _DEBUG
			fprintf(stderr, "Unable to open file %s!\n", filename);
#endif
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
		if (shaders == NULL) { return 0; }
		GLuint program = glCreateProgram();
		ShaderInfo* entry = shaders;
		while (entry->type != GL_NONE){
			GLuint shader = glCreateShader(entry->type);
			entry->shader = shader;
			const GLchar* source = ReadShader(entry->filename);
			if (source == NULL) {
				for (entry = shaders; entry->type != GL_NONE; ++entry) {
					glDeleteShader(entry->shader);
				entry->shader = 0;
			}
				return 0;
			}
			glShaderSource(shader, 1, &source, NULL);
			delete[] source;
			glCompileShader(shader);
			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
#ifdef _DEBUG
				GLsizei len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
				GLchar* log = new GLchar[len+1];
				glGetShaderInfoLog(shader, len, &len, log);
				fprintf(stderr,"SHADER COMPILATION of %s FAILED!\n%s\n", entry->filename, log);
				delete[] log;
#endif
				return 0;
			}
			glAttachShader(program, shader);
			++entry;
		}
		
		glLinkProgram(program);
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked) {
#ifdef _DEBUG
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			fprintf(stderr, "PROGRAM LINKAGE FAILED!\n%s\n", log);
			delete[] log;
#endif		
			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}
		return program;
	}
};