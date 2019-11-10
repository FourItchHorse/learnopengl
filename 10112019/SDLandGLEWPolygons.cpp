#define GLEW_STATIC
#include<D:/source/include/glew.h>
#define SDL_MAIN_HANDLED
#include<D:/source/include/SDL.h>
#include<D:/source/include/SDL_opengl.h>
#include <iostream>

const GLchar* vertexShader2DSource = R"glsl(#version 330 core
in vec2 position;
void main()
{
gl_position = vec4(position, 0, 1.0);
})glsl";

const GLchar* fragmentShaderSource = R"glsl(#version 330  core
out vec4 outColor;
void main()
{
    outColor = vec4(1.0,1.0,1.0,1.0);
 })glsl";

int main (int argc, char *argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Polygons with OpenGL with SDL", 200, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window); 
    
    glewExperimental = GL_TRUE;
    glewInit();

    
    GLuint  vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] {
        -0.05,1.0,  //top corner
        -0.6,0.25, //upper left corner
        0.6, 0.35,  //upper right corner
        //-0.45,-0.7,  //lower left corner
        //0.45, -0.7,  //lower right corner
        //0.2, 0.0,  //pinch point betweeen upper right and lower left corner
    };
    

    /*GLuint indices[] {
     0, 3, 5, 
     2, 1, 5,
     4, 1, 5
    };*/
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //creating and setting vbo as the active buffer
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copying vertices to vbo
    
    GLuint vertexShader2D = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2D, 1 , &vertexShader2DSource, NULL);
    glCompileShader(vertexShader2D);

    GLint status;
    GLchar buffer[512];
    glGetShaderiv(vertexShader2D, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader2D, 512, NULL, buffer);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader2D);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);


    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
    const char* myString = static_cast<const char*>(glGetError());
    printf();

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
        glDrawArrays(GL_TRIANGLES, 0, 3);


        SDL_GL_SwapWindow(window);
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader2D);
    glDeleteShader(fragmentShader);

    glDeleteBuffers(1, &vbo);
    
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

