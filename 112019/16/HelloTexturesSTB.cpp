#include <GL/glew.h>
#define GLEW_STATIC
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h> //NOTE TO SELF: ADD STB_IMAGE TO MINGW INCLUDE DIRECTORY ON LAPTOP


const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 TexCoord;

    void main() 
    {
        Color = color;
        TexCoord = texcoord;
        gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    }
    
)glsl";
const GLchar * fragmentShaderSource = R"glsl(
    #version 330 core
    uniform sampler2D tex;
    in vec3 Color;
    in vec2 TexCoord;
    out vec4 outColor;

    void main() 
    {
        outColor = texture(tex, TexCoord) / vec4(Color, 1.0);
    }
)glsl";

int main (int argc, char *argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Textures with STB_IMAGE.H", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar buffer[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout<<"Compiling vertex shader...\n"<<buffer<<status<<std::endl;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout<<"Compiling fragment shader ...\n"<<buffer<<status<<std::endl;

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    std::cout<<"Linking shaders to program...\n"<<buffer<<status<<std::endl;

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo); glGenBuffers(1, &ebo); 

    /*
    GLfloat vertices[] {
        -0.1, 1.0,  1.0, 0.0, 0.0,  0.5, -1.0, //top corner
        -0.7,0.25,  0.0, 1.0, 0.0,  0.0, 0.0, //upper left corner
        0.6, 0.35,  0.0, 0.0, 1.0,  1.0, 0.0 //upper right corner
        -0.45,-0.7, 0.0, 0.0, 1.0,  0.0, 1.0, //lower left corner
        0.45, -0.7, 0.25, 1.0, 0.0, 1.0, 1.0,  //lower right corner
        0.2, -0.1,  1.0, 0.5, 0.75, 0.75, 0.5//pinch point betweeen upper right and lower left corner
    }; 
                        //these were the vertices and indices for the star I had to avoid since the texcoords started screwing with its shape
    GLuint elements[] {
     4, 1, 5, 
     2, 1, 5,
     0, 3, 5, 
    }; */


    GLfloat vertices[] {
   //  Position      Color             Texcoords
    -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Top-left
     0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top-right
     0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom-left
    };

    GLuint elements[] {
        0,1,3,
        1,2,3
    }; 
 
    float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
    };

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //creating and setting vbo as the active buffer    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copying vertices to vbo
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
 
    GLuint textures[2];
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else    
    {
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3, 3, 0, GL_RGB, GL_FLOAT, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    
    

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glDeleteProgram(shaderProgram);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(2, textures);
    
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}