#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

GLuint vao, vbo, texture, program;

void CompileLinkShader(GLuint program, const GLchar* src, GLenum type)
{
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, NULL);
	glCompileShader(s);
	glAttachShader(program, s);
	glDeleteShader(s);
}
void init()
{
	const GLchar* vShaderSrc = R"glsl(
	#version 450 core
	 layout (location = 0) in vec4 position;
	void main (void)
	{
		gl_Position = position;
	}
)glsl";
	const GLchar* fShaderSrc = R"glsl(
	#version 450 core
	uniform sampler2D s;
	out vec4 color;
	void main(void)
	{
		color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);
	}	
)glsl";	

	static const float positions[] = 
	{
		0.25, -0.25, 0.5, 1.0,
	       -0.25, -0.25, 0.5, 1.0,
	        0.25, 0.25, 0.5, 1.0,
	};
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(positions), positions, 0);

	program = glCreateProgram();
	CompileLinkShader(program, vShaderSrc, GL_VERTEX_SHADER);
	CompileLinkShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		GLchar log[1024];
		glGetProgramInfoLog(program, 1024, NULL, log);
		printf("\n%s\n", log);
		delete[] log;
	}
	glUseProgram(program);

	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA32F, 256, 256);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLfloat* data = new float[256 * 256 * 4];

	glTextureSubImage2D(texture, 
			   0, 
			   0, 0,
			   256, 256,
			   GL_RGBA, 
			   GL_FLOAT, 
			   data);
	delete [] data;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
}

void display()
{
	const GLfloat color[] = {0.0, 0.5, 0.2, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
int main (int argc, char** argv) 
{
	if(!glfwInit()){ printf("Unable to init GLFW!\n"); return -1;}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Textures", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if(!window){printf("Unable to create window!\n"); return -1;}
	if(gl3wInit()){printf("Unable to init gl3w!\n"); return -1;}
	printf("VERSION: %s\nVENDOR: %s\nRENDERER: %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

	
	init();
	while(!glfwWindowShouldClose) 
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
