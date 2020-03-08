#pragma once
#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include "ShaderReader.h"

void display(GLuint* program) 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void init() 
{
	GLuint vao;  
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
}
int main() 
{
	ShaderReader::ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/08032020.vert"},
		{GL_FRAGMENT_SHADER, "shaders/08032020.frag"}
	};

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
	
	fprintf(stderr, "\nVENDOR: %s\n", (char*)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));

	GLuint program = ShaderReader::LoadShaders(shaders);
	
	while(glfwWindowShouldClose(myWindow)) 
	{
		display(&program);
		glfwSwapBuffers(myWindow);
	}
	glDeleteProgram(program);
	return 0;
}
