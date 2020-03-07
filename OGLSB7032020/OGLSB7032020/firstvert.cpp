//file initialised 07/03/2020
#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

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
	#verison 450 core
	out vec4 color;
	void main(void)
	{
		color = vec4(0.0, 0.8, 1.0, 1.0);
	}
)glsl";


GLuint compileProgram(shader* shaders) 
{
	shader* entry = shaders;
	GLuint program = glCreateProgram();
	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->ID = shader;
		glShaderSource(shader, 1, &entry->source, NULL);
		delete[] entry->source;
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		/*if (!compiled)
		{
			GLsizei len;
			GLchar* log = new GLchar[len+1];
			glGetShaderInfoLog(shader, len, &len, log);
			fprintf(stderr, "%s\n", log);
			return 0;
		} */
		glAttachShader(program, shader);
		++entry;
	}
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_COMPILE_STATUS, &linked);
	/*if (!linked)
	{
		GLsizei len;
		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		fprintf(stderr, "%s\n", log);
		return 0;
	}*/
	return program;
}
void display() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
int main(int argc, char** argv) 
{
	fprintf(stderr, "Using main from 07032020 first vert");
	int major = 4;
	int minor = 3;
	if (!glfwInit())
	{
		fprintf(stderr, "UNABLE TO CREATE WINDOW\n");
		return -1;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_VERSION_MINOR, minor);
	
	GLFWwindow* myWindow = glfwCreateWindow(800, 600, "First vert 07032020", nullptr, nullptr);

	GLuint vao;
	shader myShaders[] = {
		{vertexShader, GL_VERTEX_SHADER},
		{fragmentShader, GL_FRAGMENT_SHADER},
		{NULL, GL_NONE},
	};
	GLuint program = compileProgram(myShaders);
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(myWindow))
	{
		display();
		
		glfwSwapBuffers(myWindow);
	}

	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
} 
