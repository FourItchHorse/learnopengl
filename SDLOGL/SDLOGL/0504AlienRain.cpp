#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "OGLScene.h"
class alienRain : public OGLScene
{
public:
	void startup() 
	{
		const GLchar* vShaderSrc = R"glsl(
		
		)glsl";
		const GLchar* fShaderSrc = R"glsl(
		
		)glsl";
		program = glCreateProgram();
		compileShaders(program, vShaderSrc, GL_VERTEX_SHADER);
		compileShaders(program, fShaderSrc, GL_FRAGMENT_SHADER);
		glLinkProgram(program);
		glUseProgram(program);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);		
		
	}
	void render(float currentTime) {}
protected:
	GLuint program;
	GLuint texture;
	GLuint vao;
};
