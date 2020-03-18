#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
typedef struct  {
	const GLchar* source;
	GLenum type;
	GLuint shader;
} ShaderInfo;

const GLchar* vertexShaderSource = R"glsl(
#version 450 core
void main(void) 
{
	gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
}
)glsl"; 
const GLchar* fragmentShaderSource = R"glsl(
#version 450 core
out vec4 color;
void main(void) 
{
	color = vec4(0.0, 0.8, 1.0, 1.0);
}
)glsl";
void compileShader() 
{
	vertex_shader = glCreateShader(GL_VERTEX);
	glShaderSource(vertex_shader, 1, vertexShaderSource, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragmentShaderSource, NULL);
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
}
void shutdown() {
}
void display() {
	float currentTime = (float) glfwGetTime();
	const GLfloat color[] = {sin(currentTime) * 0.5f + 0.5f, 
				 cos(currentTime) * 0.5f + 0.5f,
				0.0, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);

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
}
