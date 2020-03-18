//file initialised 07/03/2020
#pragma once
#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>


/*
#include <stdio.h>
#include <math.h>

struct shader {
	const char* source;
	GLenum type;
	GLuint ID;
};

const GLchar* vertexShader = R"glsl(
	#version 450 core
	void main(void) 
	{
		gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
	}
)glsl";
const GLchar* fragmentShader = R"glsl(
	#version 450 core
	out vec4 color;
	void main(void)
	{
		color = vec4(0.0, 0.8, 1.0, 1.0);
	}
)glsl";

GLuint compileProgram(shader* shaders) 
{
	GLuint program;
	shader* entry = shaders;
	program = glCreateProgram();
	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->ID = shader;
		glShaderSource(shader, 1, &entry->source, NULL);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			GLchar* log = new GLchar[len];
			glGetShaderInfoLog(shader, len, &len, log);
			fprintf(stderr, "%s\n", log);
			return 0;
		} 
		glAttachShader(program, shader);
		glDeleteShader(shader);
		++entry;
	}
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_COMPILE_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len];
		glGetProgramInfoLog(program, len, &len, log);
		fprintf(stderr, "%s\n", log);
		return 0; 
	} 
	return program;
}
void display(GLuint* program, GLuint* vao) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[0]);
	glDrawArrays(GL_POINTS, 0, 1);
}
void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
	
}
int main(int argc, char** argv) 
{
	fprintf(stderr, "Using main from 07032020 first vert\n");
	int majorVersion = 4;
	int minorVersion = 3;
	if (!glfwInit())
	{
		fprintf(stderr, "UNABLE TO INIT GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* myWindow = glfwCreateWindow(800, 600, "First vert 07032020", nullptr, nullptr);
	if (!myWindow)
	{
		fprintf(stderr, "Failed to create window\n");
		return -1;
	}
	glfwMakeContextCurrent(myWindow);

	if (gl3wInit())
	{
		fprintf(stderr, "UNABLE TO INIT OPENGL: %d\n", gl3wInit());
		return -1;
	}

	fprintf(stderr, "\nVENDOR: %s\n", (char*)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char*)glGetString(GL_RENDERER));


	glPointSize(40.0f);
	GLuint vao;
	shader myShaders[] = {
		{vertexShader, GL_VERTEX_SHADER},
		{fragmentShader, GL_FRAGMENT_SHADER},
		{NULL, GL_NONE},
	};
	GLuint program = compileProgram(myShaders);
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glUseProgram(program);


	while (!glfwWindowShouldClose(myWindow))
	{
		display(&program, &vao);	
		processInput(myWindow);
		glfwSwapBuffers(myWindow);
	}

	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
} 

*/
