#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
GLuint program, vao;
static const GLchar* vertexShaderSource = R"glsl(
#version 450 core
layout (location = 0) in vec4 offset;
layout (location = 1) in vec4 color;
out VS_OUT
{
	vec4 color;
} vs_out;
void main(void) 
{
	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
				         vec4(-0.25, -0.25, 0.5, 1.0),
					 vec4(0.25, 0.25, 0.5, 1.0));
	gl_Position = vertices[gl_VertexID] + offset;
	vs_out.color = color;
}
)glsl"; 
static const GLchar* fragmentShaderSource = R"glsl(
#version 450 core
in VS_OUT
{
	vec4 color;
}fs_in;
out vec4 color;
void main(void) 
{
	color = fs_in.color;
}
)glsl";
GLuint compileProgram() 
{
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	return program;	
}
void init() {
	program = compileProgram();
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLfloat orange[] = {0.5f, 0.5f, 0.0f, 1.0f}; 
	glVertexAttrib4fv(1, orange);
}
void shutdown() {
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
}
	
void display() {
	float currentTime = (float) glfwGetTime();
	float redY = sin(currentTime) * 0.5f + 0.5f;
	float greenX = cos(currentTime) * 0.5f + 0.5f;
	const GLfloat color[] = {redY, greenX, 0.0, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
	GLfloat attrib[] = {sin(currentTime) * 0.5f,
			    cos(currentTime) * 0.6f,
			    0.0f, 0.0f};
	glVertexAttrib4fv(0, attrib);
	GLfloat orange[] = {1 - redY, 1 - greenX, 0.5, 1.0f};
	glVertexAttrib4fv(1, orange);
	glUseProgram(program);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


int main(int argc, char** argv) {
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialise GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow * window = glfwCreateWindow(600, 400, "Render BG", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if(gl3wInit()) 
	{
		fprintf(stderr, "Failed to create opengl context\n");
		return -1;
	}
	fprintf(stderr, "VENDOR: %s\n", (char*) glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char*) glGetString(GL_VERSION));	
	fprintf(stderr, "RENDERER: %s\n", (char*) glGetString(GL_RENDERER));

	init();

	while(!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		display();
		glfwSwapBuffers(window);
	}

	shutdown();
	return 0;
}
