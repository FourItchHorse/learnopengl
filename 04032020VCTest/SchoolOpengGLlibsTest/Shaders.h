
#define _CRT_SECURE_NO_WARNINGS


#include <GL/gl3w.h>
#include <cstdlib>
#include <iostream>

typedef struct {
	GLenum			type;
	const char* filename;
	GLuint 	      shader;
} ShaderInfo;


static const GLchar ReadShaders(const GLchar*);
GLuint LoadShaders(ShaderInfo*);

