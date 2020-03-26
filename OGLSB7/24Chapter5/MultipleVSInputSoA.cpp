#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <vmath/vmath.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
GLuint program, vao, buffer[2];
static const GLchar* vertexShaderSource = R"glsl(
#version 450 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 colour;
out vec3 vsCol;
void main (void)
{
	gl_Position = position;
	vsCol = colour;
}
)glsl";
static const GLchar* fragmentShaderSource = R"glsl(
#version 450 core
in vec3 vsCol;
out vec4 outCol;
void main (void) 
{
	outCol = vec4(vsCol, 1.0);
}
)glsl";
void compileShader(const char* source, GLenum type, GLuint program) 
{
	GLuint myShader = glCreateShader(type);
	glShaderSource(myShader, 1, &source, NULL);
	glCompileShader(myShader);
	GLint compiled;
	glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
	GLchar log[1024];
	glGetShaderInfoLog(myShader, 1024, NULL, log);
	fprintf(stderr,"\n%s\n", log);
	delete [] log;
	}
	glAttachShader(program, myShader);
	glDeleteShader(myShader);

}
void linkProgram(GLuint program) 
{
	glLinkProgram(program);
	int linked;
	glGetShaderiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
	GLchar log[1024];
	glGetProgramInfoLog(program, 1024, NULL, log);
	fprintf(stderr,"\n%s\n", log);
	delete [] log;
	}	
}
void init() {
	program = glCreateProgram();
        compileShader(vertexShaderSource, GL_VERTEX_SHADER, program); 
	compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER, program);
	linkProgram(program);	
	static const float positions[] = 
	{
			0.25, -0.25, 0.5, 1.0,
	       -0.25, -0.25, 0.5, 1.0,
	        0.25, 0.25, 0.5, 1.0,
	};
	static const float colours[] = 
	{
		0.1, 0.5, 0.0, 
		0.5, 1.0, 0.5,  
		0.0, 0.5, 1.0, 
	};
	glCreateVertexArrays(1, &vao);
	glCreateBuffers(2, buffer); 
	
	glNamedBufferStorage(buffer[0], sizeof(positions), positions, 0);
	glVertexArrayVertexBuffer(vao, 0, buffer[0], 0, sizeof(vec4));
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glEnableVertexArrayAttrib(vao, 0);

	glNamedBufferStorage(buffer[1], sizeof(colours), colours, 0);
	glVertexArrayVertexBuffer(vao, 1, buffer[1], 0, sizeof(vec3));
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 1, 1);
	glEnableVertexArrayAttrib(vao, 1);

	glBindVertexArray(vao);
       	
}
void shutdown() {
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
}
	
void display() {
	const GLfloat color[] = {0.0, 0.5, 0.2, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(program);
	glBindVertexArray(vao);
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
