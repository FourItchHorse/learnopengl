#define GLEW_STATIC
#include<D:/source/include/glew.h>
#define SDL_MAIN_HANDLED
#include<D:/source/include/SDL.h>
#include<D:/source/include/SDL_opengl.h>
#include <iostream>

const GLchar* vertexShaderSource = R"glsl(#version 330 core 
layout (location = 0) in vec3 position;
void main()
{
gl_Position = vec4(position.x, position.y, position.z, 1.0);
})glsl";

const GLchar* fragmentShaderSource = R"glsl(#version 330 core
out vec4 color;
void main()
{
color = vec4(1.0,1.0,1.0,1.0);
 })glsl";

int main (int argc, char *argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Polygons with GLEW and SDL", 200, 100, 800, 600, SDL_WINDOW_OPENGL);
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
    glBindFragDataLocation(shaderProgram, 0, "color");
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    
    std::cout<< "Linking shaders to program...\n" << buffer;

    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint vao, vbo, ebo;
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    GLfloat vertices[] {
        -0.1,1.0, 0, //top corner
        -0.7,0.25, 0,//upper left corner
        0.6, 0.35,  0, //upper right corner
        -0.45,-0.7, 0,  //lower left corner
        0.45, -0.7, 0, //lower right corner
        0.2, -0.1, 0, //pinch point betweeen upper right and lower left corner
    };
    

    GLuint elements[] {
     0, 3, 5, 
     2, 1, 5,
     4, 1, 5
    };
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //creating and setting vbo as the active buffer    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copying vertices to vbo
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    printf("OpenGL error: %u\n", glGetError());

    SDL_Event windowEvent;
    while(true) 
    {   
        
        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_QUIT) { break; }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE){  break; }        
        }

        glClearColor(0.0f,0.0f,0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }


    glDeleteBuffers(1, &vbo);
    
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

