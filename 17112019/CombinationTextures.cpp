#include <GL/glew.h>
#define GLEW_STATIC
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb_image.h>

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

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    uniform sampler2D tex1;
    uniform sampler2D tex2;
    uniform float texWeight;
    uniform float tanTime;
    in vec3 Color;
    in vec2 TexCoord;
    out vec4 outColor;

    void main() 
    {
        vec4 colTex1 = texture(tex1, TexCoord);
        vec4 colTex2 = texture(tex2, TexCoord * tanTime);
        outColor = vec4(1 - Color, 1);
        outColor = mix(colTex1, colTex2, texWeight) * outColor;
    }
)glsl";

int main(int argc, char *argv[]) 
{
    //create window and context
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Mixing textures", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();
    
    //compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar buffer[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout<<"VERTEX SHADER COMPILE STATUS: "<<status<<" VERTEX SHADER INFOLOG: \n"<< buffer;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    std::cout << "FRAGMENT SHADER COMPILE STATUS: " << status << " FRAGMENT SHADER INFO LOG: \n" << buffer;

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    std::cout << "SHADER PROGRAM LINK STATUS: "<<status<< " SHADER PROGRAM INFO LOG\n " << buffer;
    
    //create and bind to vertexArrayObject, vertexBufferObject, elementBufferObject

    GLuint vao, vbo, ebo, textures[2];
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);
    GLfloat vertices[] 
    { //Position    Color         TexCoords
     -0.5, 0.5,   1.0, 0.0, 0.0,  0.0, 0.0,
      0.5, 0.5,   0.0, 1.0, 0.0,  1.0, 0.0,
      0.5,-0.5,   0.0, 0.0, 1.0,  1.0, 1.0,
     -0.5,-0.5,   1.0, 1.0, 1.0,  0.0, 1.0, 
    };

    GLuint indices[] 
    {
        0, 1, 3,
        1, 2, 3
    };

     float pixels[] = {
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
    };

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //create, bind and feed images to texture objects  
    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "FAILED TO LOAD TEXTURE 1\n";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels);
    } 
    else if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    data = stbi_load("Textures/cat.png", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "FAILED TO LOAD TEXTURE 2\n";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels);
    }
    else if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
    
    //create and enable vertexAttrib arrays from in variables in shaderProgram
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));

    //render loop
    printf("OPENGL ERROR: %u\n", glGetError());
    SDL_Event windowEvent;
    float texMix = 0.0f;
    float texLimit = 1.0;
    float tanTime;
    while (true)
    {
        if(SDL_PollEvent(&windowEvent)) 
        {
            if (windowEvent.type == SDL_QUIT) {  break; }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) { break; }
            if (windowEvent.type == SDL_KEYDOWN) 
            {
                if (texMix < texLimit && windowEvent.key.keysym.sym == SDLK_UP)
                {
                    texMix += 1.0f/512.0f;
                }
                else if (texMix >= 0 && windowEvent.key.keysym.sym == SDLK_DOWN)
                {
                    texMix -= 1.0f/512.0f;
                }
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(shaderProgram, "texWeight"), texMix);
        glUniform1f(glGetUniformLocation(shaderProgram, "tanTime"), tanTime);
        tanTime = tan(static_cast<float>(SDL_GetTicks()) / (texMix * 4096.0f));
        printf("WEIGHT: %f TAN TIME: %f\r", texMix, tanTime);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0); 
        glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    //unbind and delete array and buffer objects
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(2, textures);
    glDeleteVertexArrays(1, &vao);

    //delete shaderProgram, vertexShader, fragmentShader and context
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glDeleteProgram(shaderProgram);
    

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;

}