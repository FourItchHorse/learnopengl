#define _USE_MATH_DEFINES 1
#include "BaseLib.h"
#include "ShaderReader.h"
class my_first_tri : public OGLApp
{
public:
	void startup()
	{
		ShaderInfo myShaders[] = {
			{GL_VERTEX_SHADER, "1204triVshader.glsl"},
			{GL_FRAGMENT_SHADER, "1204triFshader.glsl"},
			{GL_NONE, NULL},
		};
		program = LoadShaders(myShaders);
		glUseProgram(program);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		colAttrib = glGetUniformLocation(program, "inCol");
		glBindVertexArray(vao);
	}
	void render(float currentTime)
	{
		float redX = (float)sin(currentTime) * 0.5f + 0.5f;
		float greenY = (float)cos(currentTime) * 0.5f + 0.5f;
		glClearColor(redX, greenY, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		GLfloat col[] = {1 - redX, 1 - greenY, 0.0};
		glUniform3fv(colAttrib, 1, col);
		glVertexAttrib3f(1, cos(currentTime), sin(currentTime), 0.0);
		glVertexAttrib4f(2, 0.0, 0.0, 1.0, -M_2_PI * currentTime);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
protected:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLint colAttrib;
};

int main (int argc, char** argv)
{
	my_first_tri *scene = new my_first_tri();
	scene->run(scene);
	delete scene;
	return 0;
} 