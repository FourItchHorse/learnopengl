#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stb/stb_image.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

const GLchar * vertexShaderSource =  R"glsl(

)glsl";
const GLchar * fragmentShaderSource = R"glsl(

)glsl";

GLuint createShaderProgram(const GLchar * vertex, const GLchar * fragment);
void mapActiveTexture(const char* filename);
void pointToVertexAttrib(GLint attrib, int stepSize, int arraySize, int offset);

glm::mat4 modelTransform();
glm::mat4 viewTransform();
glm::mat4 modelTransform();

int main () 
{

}