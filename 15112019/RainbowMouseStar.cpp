#define GLEW_STATIC
#include<GL/glew.h>
#define SDL_MAIN_HANDLED

#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include <iostream>
#include <cmath>

const GLchar* vertexShaderSource = R"glsl(#version 330 core 
in vec2 position;
in vec2 texcoord;
in vec3 color;
out vec3 Color;
out vec2 TexCoord;
void main()
{
Color = color;
gl_Position = vec4(position.x, position.y, 0.0, 1.0);
})glsl";

const GLchar* fragmentShaderSource = R"glsl(#version 330 core
uniform vec3 mouse;
in vec3 Color;
out vec4 outColor;
void main()
{
outColor = vec4(mouse / Color ,1.0);
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
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    
    std::cout<< "Linking shaders to program...\n" << buffer;

    
    GLuint vao, vbo, ebo, tex;
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenTextures(1, &tex);

    GLfloat vertices[] {
        -0.1, 1.0,  1.0, 0, 0, //top corner
        -0.7,0.25,  0.0, 1.0, 0, //upper left corner
        0.6, 0.35,  0.0, 0.0, 1.0, //upper right corner
        -0.45,-0.7, 0.0, 0.0, 1.0, //lower left corner
        0.45, -0.7, 0.25, 1.0, 0.0, //lower right corner
        0.2, -0.1,  1.0, 0.5, 0.75 //pinch point betweeen upper right and lower left corner
    };
    

    GLuint elements[] {
     4, 1, 5, 
     2, 1, 5,
     0, 3, 5, 
    };
    
    float pixels[] { 0.0f, 0.0, 0.0, 1.0, 1.0, 1.0, 
                     1.0, 1.0, 1.0, 0.0, 0.0, 0.0
                    };


    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //creating and setting vbo as the active buffer    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copying vertices to vbo
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    
    printf("OpenGL error: %u\n", glGetError());

    SDL_Event windowEvent;
    while(true) 
    {   
        int x, y;
        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_QUIT) { break; }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE){  break; }
            SDL_GetMouseState(&x, &y);       
        }

        glClearColor(0.0f,0.0f,0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float mouseX = static_cast<double>(x);
        float mouseY = static_cast<double>(y);
        float denominator = 5000.0f;
        float hypotenuse = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
        glUniform3f(glGetUniformLocation(shaderProgram, "mouse"), mouseX / denominator, mouseY/ denominator, hypotenuse/ denominator);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);     
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        printf("R: %f G: %f B %f \r", mouseX, mouseY, hypotenuse);

        SDL_GL_SwapWindow(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glDeleteProgram(shaderProgram);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

