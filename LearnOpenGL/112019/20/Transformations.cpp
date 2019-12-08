#include <GL/glew.h>
#define GLEW_STATIC
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb_image.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    in vec2 texcoord2;
    out vec3 Color;
    out vec2 TexCoord;
    out vec2 TexCoord2;
    uniform mat4 transform;

    void main() 
    {
        Color = color;
        TexCoord = texcoord;
        TexCoord2 = texcoord2;
        
        gl_Position =  transform * vec4(position.x, position.y, 0.0, 1.0);
    }

)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    uniform sampler2D tex1;
    uniform sampler2D tex2;
    uniform float texWeight;
    uniform mat4 transform;
    in vec3 Color;
    in vec2 TexCoord;
    in vec2 TexCoord2;
    out vec4 outColor;

    void main() 
    {
        vec4 colTex1 = texture(tex1, TexCoord);
        vec4 colTex2 = texture(tex2, TexCoord2);
        outColor = mix(colTex1, colTex2, texWeight) * (transform * vec4(Color, 1.0));
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

    SDL_Window* window = SDL_CreateWindow("Transformations with GLM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

    glUseProgram(shaderProgram);
    
    //create and bind to vertexArrayObject, vertexBufferObject, elementBufferObject

    GLuint vao, vbo, ebo, textures[2];
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);
    GLfloat vertices[] 
    { //Position    Color         TexCoords
     -0.5, 0.5,   1.0, 0.0, 0.0,  0.0, 0.0,  0.0, 0.0,
      0.5, 0.5,   0.0, 1.0, 0.0,  1.0, 0.0,  5.0, 0.0,
      0.5,-0.5,   0.0, 0.0, 1.0,  1.0, 1.0,  5.0, 10.0,
     -0.5,-0.5,   1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 10.0,
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
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
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) 0);
    printf("POSITION ATTRIB OPENGL ERROR: %u\n", glGetError());
   
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    printf("COLOR ATTRIB OPENGL ERROR: %u\n", glGetError());

    
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    printf("TEXCOORD1 ATTRIB OPENGL ERROR: %u\n", glGetError());
    
    
    GLint texAttrib2 = glGetAttribLocation(shaderProgram, "texcoord2");
    glEnableVertexAttribArray(texAttrib2);
    glVertexAttribPointer(texAttrib2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (7 * sizeof(GLfloat)));
    printf("TEXCOORD2 ATTRIB OPENGL ERROR: %u\n", glGetError()); 
    
    //render loop
    printf("OPENGL ERROR: %u\n", glGetError());
    SDL_Event windowEvent;
    float inputFloat = 0.5f;
    float floatLimit = 360.0f;
    float time;
    while (true)
    {
        if(SDL_PollEvent(&windowEvent)) 
        {
            if (windowEvent.type == SDL_QUIT) {  break; }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) { break; }
            if (windowEvent.type == SDL_KEYDOWN) 
            {
                if (inputFloat < floatLimit && windowEvent.key.keysym.sym == SDLK_UP)
                {
                    inputFloat += 1.0f/512.0f;
                }
                else if (inputFloat >= 0 && windowEvent.key.keysym.sym == SDLK_DOWN)
                {
                    inputFloat -= 1.0f/512.0f;
                }
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(shaderProgram, "texWeight"), inputFloat);
        
        time = static_cast<float>(SDL_GetTicks())/ 1024.0f;
        GLint uniTransform = glGetUniformLocation(shaderProgram, "transform");
        
        glm::mat4 timeRotation = glm::mat4(1.0f);
        timeRotation = glm::rotate(
            timeRotation,
            time * glm::radians(90.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glm::mat4 inputRotation = glm::mat4(1.0f);
        inputRotation = glm::rotate(
            inputRotation,
            inputFloat * glm::radians(180.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(timeRotation));

        printf("WEIGHT: %f TIME: %f\r", inputFloat, time);
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