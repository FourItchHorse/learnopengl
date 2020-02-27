#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

typedef struct { 
        GLenum		type;
	const char* filename;
	GLuint 	      shader;
} ShaderInfo;

static const GLchar* ReadShader(const char* filename) 
{
	FILE* infile = fopen (filename, "rb" );
	if(!infile) 
	{
	#ifdef _DEBUG
		std::cerr << "Unable to open file '"<< filename << ",'" << std::endl;
	#endif /*_DEBUG*/
		return NULL
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell (infile);
	fseek (infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}
GLuint LoadShaders(ShaderInfo* shaders) 
{
	/*github.com/openglredbook/examples/blob/master/lib/LoadShaders.cpp*/

	if( shaders == NULL ) {return 0; }

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	
}

void init (void) 
{
	static const GLfloat vertices[NumVertices][2] =
	{
		{-0.9, -0.9},
		{0.85, -0.9},
		{-0.9, 0.85},
		
		{0.9, -0.85},
		{0.9, 0.9},
		{-0.85, 0.9}
	};

	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer],
		sizeof(vertices), 0);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "27022020.vert" },
		{GL_FRAGMENT_SHADER, "27022020.frag" },
		{GL_NONE, NULL }
	};
}
void display(void)
{

}

int main(int argc, char** argv) 
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "Triangles", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	
	init();

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
