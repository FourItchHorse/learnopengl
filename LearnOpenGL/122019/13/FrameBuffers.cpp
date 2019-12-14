#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATIOn

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

using namespace glm;

void MainLoop() 
{
    SDL_Event windowEvent;
    do 
    {

    } while (windowEvent.type != SDL_QUIT);
}



char infoLog[512];
const char* ConvertShaderSource(const char* filePath) 
{
    std::ifstream shaderFile;
    int length;
    shaderFile.open(filePath);
    std::string buffer, line;
    while (shaderFile)
    {
        std::getline(shaderFile, line);
        buffer += line + "\n";
    }
    buffer += "\r";
    std::cout << "My shader string is..." << buffer << std::endl;
    shaderFile.close();
    const char* data = buffer.c_str();
    std::cout << "My shader character is..." << data << std::endl;
    return data;
}


const char* vShaderSrc = ConvertShaderSource("C:\\source\\repos\\C++\\learnopengl\\LearnOpenGL\\122019\\13\\vShader.vs");
const char* fShaderSrc = ConvertShaderSource("C:\\source\\repos\\C++\\learnopengl\\LearnOpenGL\\122019\\13\\fShader.fs");

GLuint LoadShader(const char* shaderSrc, GLenum type)
{ 
    GLuint myShader = glCreateShader(type);
    
    if (myShader == 0) return 0;
    glShaderSource(myShader, 1, &shaderSrc, NULL);

    glCompileShader(myShader);
    GLint compiled;
    /*glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) 
    {
        glGetShaderInfoLog(myShader, 512, NULL, infoLog);
        printf("FAILED TO COMPILE\n%s\n", infoLog);
        glDeleteShader(myShader);
        return 0;
    } */

    return myShader;
}
GLuint InitProgram(const char* vShaderSrc, const char* fShaderSrc)
{
    GLuint vShader = LoadShader(vShaderSrc, GL_VERTEX_SHADER);
    GLuint fShader = LoadShader(fShaderSrc, GL_FRAGMENT_SHADER);
    GLuint myProgram = glCreateProgram();

    glLinkProgram(myProgram);
    int linked;
    glGetProgramiv(myProgram, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(myProgram, 512, NULL, infoLog);
        printf("FAILED TO LINK\n%s\n", infoLog);
        glDeleteProgram(myProgram);
        return 0;
    }

    return myProgram;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint shaderProgram = InitProgram(vShaderSrc, fShaderSrc);
    //printf("The shader sources are... \n%s\n%s\n", vShaderSrc, fShaderSrc);
}