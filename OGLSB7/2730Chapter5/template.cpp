#include <GL/gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#include <vmath/vmath.h>
#include <stdio.h>
GLuint vbo, vao, program;
GLint mvLoc, prjLoc, compiled;
mat4 mv_matrix, proj_matrix;
void compileShader(GLuint program, const GLchar* source, GLenum type) {
		GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &source, NULL);
        glCompileShader(s);
		glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
		glAttachShader(program, s);
}
void init() 
{
	const GLchar* vShaderSrc = R"glsl(
		#version 450 core
		in vec4 position;
		out VS_OUT
		{
			vec4 color;
		} vs_out;
		uniform mat4 mv_matrix;
		uniform mat4 proj_matrix;
		void main(void)
		{
			gl_Position = proj_matrix * mv_matrix * position;
			vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
		} 
	)glsl";
	const GLchar* fShaderSrc = R"glsl(
		#version 450 core
		out vec4 color;
		in VS_OUT
		{
			vec4 color;
		} fs_in;
		void main (void) {
			color = fs_in.color;
		}
	)glsl";
	program = glCreateProgram();
	compileShader(program, vShaderSrc, GL_VERTEX_SHADER);
	compileShader(program, fShaderSrc, GL_FRAGMENT_SHADER);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
		GLchar Log[1024];
		glGetProgramInfoLog(program, 1024, NULL, Log);
		printf("\n%s\n", Log);
	glUseProgram(program);
	
	mvLoc = glGetUniformLocation(program, "mv_matrix");
	prjLoc = glGetUniformLocation(program, "proj_matrix");
	printf("\n%i, %i \n", mvLoc, prjLoc);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertex_data[] = {
			-0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(vao);
	mv_matrix = translate(0.0f, 0.0f, -20.0f);
}
void display() 
{
	glClearColor(0.0, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(prjLoc, 1, GL_FALSE, proj_matrix);
	
	float f = (float)glfwGetTime() * 0.03f;
	mv_matrix = translate(0.0f, 0.0f, -20.0f) *
         rotate(0.0f, 1.0f, 0.0f, (float)glfwGetTime() * 4.5f) * 
	 rotate(1.0, 0.0, 0.0, (float)glfwGetTime() * 2.1f) * 
		translate(sinf(2.1 * f) * 2.0f,
		cosf(1.7f * f) * 2.0f,
		sinf(1.3 * f) * cosf(1.5 * f) * 2.0f);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv_matrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void shutdown() 
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
} 
int main (int argc, char** argv)
{
	if(!glfwInit())
	{
		printf("Unable to init GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* myWindow = glfwCreateWindow(800, 600, "Spinning Cube", nullptr, nullptr);
	if(!myWindow) 
	{
		printf("Unable to create window!\n");
		return -1;
	}
	glfwMakeContextCurrent(myWindow);
	if (gl3wInit())
	{
		printf("Unable to create Opengl context!\n");
		return -1;
	}
	printf("\nGL_VERSION: %s\nGL_VENDOR: %s\nGL_RENDERER: %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	
	init(); 

	GLint width, height;
	glfwGetWindowSize(myWindow, &width, &height);
	float aspect = (float) width/(float) height;
	proj_matrix = perspective(50.0f, aspect, 0.1f, 1000.0f);
	while(!glfwWindowShouldClose(myWindow)) 
	{
		glfwPollEvents();
		display();
		glfwSwapBuffers(myWindow);
	}
	shutdown();
}
