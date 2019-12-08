
#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include <SDL2/SDL.h>
#include <SDL2\SDL_opengl.h>

#include <STB\stb_image.h>

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const GLchar* vertexShaderSource = R"glsl(
#version 330 core
in vec2 position;
in vec3 color;
in vec2 texcoord1;
in vec2 texcoord2;

out vec3 Color;
out vec2 TexCoord1;
out vec2 TexCoord2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main () 
{
    Color = color;
    TexCoord1 = texcoord1;
    TexCoord2 = texcoord2;
    
    gl_Position = proj * view  * model * vec4(position, 0.0, 1.0);
}
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 Color;
in vec2 TexCoord1;
in vec2 TexCoord2;

out vec4 outColor;

uniform float time;
uniform mat4 colorEffect;
uniform mat4 texEffect;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main () 
{
    vec4 colTex1 = texture(tex1, TexCoord1);
    vec4 colTex2 = texture(tex2, TexCoord2 + time) * texEffect;
    outColor = mix(colTex1, colTex2, 0.5) / (colorEffect * vec4(Color, 1.0));
}
)glsl";

int main (int argc, char *argv[]) 
{
    //Init, setup SDL_GL and GLEW
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    SDL_Window* window = SDL_CreateWindow("Further Transformations with GLM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    printf("INITIALISING: OPENGL ERROR: %u\n", glGetError());

    //create shader ints and compile, link and use shaderProgram
    GLint status;
    GLchar buffer[512];

    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout << buffer << std::endl;
    printf("COMPILING VERTEX SHADER ERROR: %u\n", glGetError());
    
    fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    std::cout << buffer << std::endl;
    printf("COMPILING FRAGMENT SHADER ERROR: %u\n", glGetError());
    

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    glUseProgram(shaderProgram);

    std::cout <<  status << buffer << std::endl;
    
    printf("LINKING SHADER PROGRAM ERROR: %u\n", glGetError());
    //create object ints
    GLuint vbo, ebo, vao, textures[2];
    glGenVertexArrays(1, &vao);
    //create data for objects
    GLfloat vertices[] 
    {   //position  //color         //texcoords
         0.0, -0.75,    1.0, 0.0, 0.0,    0.5, -1.0,     2.5,-10.0,    
       -0.15, -0.25,    0.0, 1.0, 0.0,   -0.1,  0.0,     0.0,  0.0,
        0.15, -0.25,    0.0, 0.0, 1.0,    0.1,  0.0,    3.75,  0.0,
       -0.65, -0.25,    0.0, 1.0, 0.0,   -1.0,  0.0,     5.0,  0.0, 
        0.65, -0.25,    1.0, 0.0, 0.0,    1.0,  0.0,     5.0,  0.0, 
       -0.20,   0.0,    0.0, 1.0, 0.0,  -0.15, 0.25,     1.0,  2.5, 
        0.20,   0.0,    0.0, 0.0, 1.0,   0.15, 0.25,     1.0,  2.5,
         0.0,  0.15,    0.0, 1.0, 0.0,    0.5, 0.45,     2.5, 10.0,   
       -0.45,   0.5,    1.0, 0.0, 0.0,   -1.0,  1.0,     5.0, 10.0,
        0.45,   0.5,    0.0, 1.0, 0.0,    1.0,  1.0,     5.0, 10.0,
    };

    GLuint indices[] 
    {
        1, 0, 2,
        3, 1, 5,
        7, 5, 1,
        1, 2, 7,
        4, 2, 6,
        7, 6, 2,
        5, 8, 7,
        7, 6, 9
    };

    //bind to and write data to objects
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    GLint width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
    if  (!data)
    {
        std::cout<<"FAILED TO LOAD TEXTURE1\n";
    }
    else if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
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
    if(!data) 
    {
        std::cout<<"FAILED TO LOAD TEXTURE2\n";
    }
    else if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
    
    
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    //creata and enable vertexAttrib pointers to ins in vertex Shader

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) 0);
    printf("POSITION ATTRIB OPENGL ERROR: %u\n", glGetError());
    

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));printf("OPENGL ERROR: %u\n", glGetError());
    printf("COLOR ATTRIB OPENGL ERROR: %u\n", glGetError());

    
    GLint texAttrib1 = glGetAttribLocation(shaderProgram, "texcoord1");
    glEnableVertexAttribArray(texAttrib1);
    glVertexAttribPointer(texAttrib1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    printf("TEXCOORD1 ATTRIB OPENGL ERROR: %u\n", glGetError());
    
    
    GLint texAttrib2 = glGetAttribLocation(shaderProgram, "texcoord2");
    glEnableVertexAttribArray(texAttrib2);
    glVertexAttribPointer(texAttrib2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (7 * sizeof(GLfloat)));
    printf("TEXCOORD2 ATTRIB OPENGL ERROR: %u\n", glGetError()); 
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f, 1.2f, 1.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), 
        800.0f/ 600.0f,
        1.0f,
        5.0f
    );
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    //render loop
    SDL_Event windowEvent;
    while (true)
    {    
        int x, y;
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                break;
            }
            SDL_GetMouseState(&x, &y);
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
 


        float mouseX, mouseY;
        mouseX = static_cast<float>(x);
        mouseY = static_cast<float>(y);
        float mouseRotation = (mouseX - 400.0f)/400.0f;
        float mouseCameraAngle = (600 - mouseY) * 45.0f/600.0f;
        float mouseCameraLookAt = (600 - mouseY) * 1.0f/600.0f;
        float time = SDL_GetTicks()/ 1024.0f;

        GLint uniModel = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
        model, 
        mouseRotation * glm::radians(45.0f), 
        glm::vec3(0.0, 0.0, 1.0)
        );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        GLint uniTex = glGetUniformLocation(shaderProgram, "texEffect");
        glm::mat4 texture = glm::mat4(1.0f);
        texture = glm::translate(
            texture,
            glm::vec3(time, time * time, time * time * time)
        );
        glUniformMatrix4fv(uniTex, 1, GL_FALSE, glm::value_ptr(texture));

        GLint colTrans = glGetUniformLocation(shaderProgram, "colorEffect");
        glm::mat4 color = glm::mat4(1.0f);
        color = glm::translate (
            color, 
            glm::vec3(0.25 * sin(time), 0.5 * sin(time), sin(time))
        );
        glUniformMatrix4fv(colTrans, 1, GL_FALSE, glm::value_ptr(color));
        
        
        GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

        GLint uniView = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    
         
        printf("TIME: %f\r", time);
        printf("MOUSE X: %f \t MOUSE Y: %f \t MODEL ROTATION SCALAR: %f \t CAMERA ANGLE: %f \t CAMERA Z POS: %f \r", 
        mouseX, mouseY, mouseRotation, mouseCameraAngle, mouseCameraLookAt);
        glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0); 
        glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
   
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
   
    }
    //delete shaders and shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    
    //unbind and delete objects
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(2, textures);
    glDeleteVertexArrays(1, &vao);
    
    //delete context
    SDL_Quit();
    SDL_GL_DeleteContext(context);
    
}