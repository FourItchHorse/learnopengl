

#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include "ShaderReader.h"
#include "../../vmath/vmath.h"

GLuint vao;
GLuint program;


void init() 
{

	ShaderReader::ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/08032020.vert"},
		{GL_FRAGMENT_SHADER, "shaders/08032020.frag"},
		{GL_NONE, nullptr}
	};
	GLuint program = ShaderReader::LoadShaders(shaders);
	glUseProgram(program);

	vec2 vertices[] = {
		vec2(0.35, -0.35),
		vec2(-0.35, -0.35),
		vec2(0.35, 0.35)
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

void display()
{
	const GLfloat Color[] = { 0.0, 0.2, 0.0, 1.0 };
	glClearBufferfv(GL_COLOR, 0, Color);
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() 
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to launch GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* myWindow  = glfwCreateWindow(800, 600, "first triangle", nullptr, nullptr);
	if(!myWindow) 
	{
		fprintf(stderr, "Failed to open window!\n");
		return -1;
	}
	glfwMakeContextCurrent(myWindow);

	if(gl3wInit()) 
	{
		fprintf(stderr, "Failed to load opengl! %d \n", gl3wInit());
		return -1;
	}

	init();
	
	fprintf(stderr, "\nVENDOR: %s\n", (char*)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));

	while(!glfwWindowShouldClose(myWindow)) 
	{
		display();
		glfwPollEvents();
		glfwSwapBuffers(myWindow);
	}
	glDeleteProgram(program);
	return 0;
}
