#define GLEW_STATIC
#include<D:/source/include/glew.h>
#define SDL_MAIN_HANDLED
#include<D:/source/include/SDL.h>
#include<D:/source/include/SDL_opengl.h>
#include <iostream>

const GLchar* vertexShaderSource = R"glsl(
#version 330 core 
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
out vec3 Color;
void main()
{
    Color = color;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 Color;
out vec4 outColor;
void main()
{
outColor = vec4(Color,1.0);
 })glsl";


int main (int argc, char *argv[]) 
{   
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("11112019", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1 , &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar buffer[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

    std::cout<<"Compiling vertex shader...\n" << buffer;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);

    std::cout<<"Compiling fragment shader...\n" << buffer;

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    
    std::cout<< "Linking shaders to program...\n" << buffer;

    GLuint vao, vbo, ebo;
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    GLfloat vertices[] {
       -0.1, 1.0, 0.5, 0, 0, //top corner
        -0.7,0.25, 0.0, 0.5, 0, //upper left corner
        0.6, 0.35, 0.0, 0.0, 0.5,  //upper right corner
        -0.45,-0.7, 1.0, 0.0, 0,  //lower left corner
        0.45, -0.7, 0.0, 1.0, 0.0, //lower right corner
        0.2, -0.1,  0.0, 0.0, 1.0 //pinch point betweeen upper right and lower left corner
 
    };

    GLuint elements[] {
        4, 1, 5, 
        2, 1, 5, 
        0, 3, 5    
    };

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glBindVertexArray(vao);

    SDL_Event windowEvent;
    while (true)
    {
        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_QUIT) { break; }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) { break; }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


        SDL_GL_SwapWindow(window);
    }

    glBindVertexArray(0);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glDeleteProgram(shaderProgram);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;

}