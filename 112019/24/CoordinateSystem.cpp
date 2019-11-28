#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include <SDL2/SDL.h>
#include <SDL2\SDL_opengl.h>

#include <STB/stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

const GLchar * vertexShaderSource = R"glsl(
    #version 330 core
    in vec3 position;
    in vec3 color;
    in vec2 texcoord1;

    out vec3 Color;
    out vec2 Texcoord1;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() 
    {
        Color = color;
        Texcoord1 = texcoord1;

        gl_Position = proj * view * model * vec4(position, 1.0);
    }
)glsl";

const GLchar * fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 Color;
    in vec2 Texcoord1;

    out vec4 outColor;

    uniform sampler2D tex1;
    uniform sampler2D tex2;

    void main () 
    {
        vec4 colTex1 = texture(tex1, Texcoord1);
        vec4 colTex2 = texture(tex2, Texcoord1);
        outColor = mix(colTex1, colTex2, 0.5) * (vec4(Color, 1.0));
    }
)glsl";


int main () 
{
    
    SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_Window* window = SDL_CreateWindow("Coordinate System", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(window);

        glewExperimental = GL_TRUE;
        glewInit();


    GLint status;
    GLchar buffer[512];    
    GLuint vertexShader, fragmentShader, shaderProgram;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
            std::cout << buffer << std::endl;
            printf("VERTEX SHADER OPENGL ERROR: %u\n", glGetError());

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
            std::cout << buffer << std::endl;
            printf("FRAGMENT SHADER OPENGL ERROR: %u\n", glGetError());

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
            std::cout << buffer << std::endl;
            glUseProgram(shaderProgram);
            printf("SHADER OPENGL ERROR: %u\n", glGetError());

    GLuint vbo, vao, ebo, textures[2];
    glGenVertexArrays(1, &vao);
    GLfloat vertices[] {
        -0.5, 0.5, -0.5,    1.0, 0.0, 0.0,  0.0, 0.0,
         0.5, 0.5, -0.5,    0.0, 1.0, 0.0,  1.0, 0.0,
         0.5,-0.5, -0.5,    0.0, 0.0, 1.0,  1.0, 1.0,
        -0.5,-0.5, -0.5,    0.0, 1.0, 1.0,  0.0, 1.0, 
        -0.5, 0.5,  0.5,    1.0, 0.0, 0.0,  0.0, 0.0,
         0.5, 0.5,  0.5,    0.0, 1.0, 0.0,  1.0, 0.0,
         0.5,-0.5,  0.5,    0.0, 0.0, 1.0,  1.0, 1.0,
        -0.5,-0.5,  0.5,    0.0, 1.0, 0.0,  0.0, 1.0,
    
    };
    GLuint indices[] {
        0, 1, 3,
        1, 2, 3, 
        1, 7, 3,
        3, 6, 7,
        0, 2, 5,
        2, 4, 5,
        2, 3, 7,
        3, 5, 7, 
        4, 6, 7, 
        4, 5, 7,
        1, 3, 7, 
        1, 5, 7
    };
    
    glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            printf("VERTEX BUFFER OPENGL ERROR: %u\n", glGetError());

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            printf("ELEMENT BUFFER OPENGL ERROR: %u\n", glGetError());


    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    GLint width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
        if(data) 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else 
        {
            std::cout << "FAILED TO LOAD TEXTURE 1!" << std::endl;
        }
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    data = stbi_load("Textures/cat.png", &width, &height, &nrChannels, 0);if(data) 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else 
        {
            std::cout << "FAILED TO LOAD TEXTURE 2!" << std::endl;
        }
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);

        GLint uniModel = glGetUniformLocation(shaderProgram, "model");

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);
    printf("POSITION ATTRIB OPENGL ERROR: %u\n", glGetError());
    

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    printf("COLOR ATTRIB OPENGL ERROR: %u\n", glGetError());

    
    GLint texAttrib1 = glGetAttribLocation(shaderProgram, "texcoord1");
    glEnableVertexAttribArray(texAttrib1);
    glVertexAttribPointer(texAttrib1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    printf("TEXCOORD1 ATTRIB OPENGL ERROR: %u\n", glGetError());
    
    
    glm::mat4 model = glm::mat4(1.0f);
    
    glm::mat4 proj = glm::perspective(
            glm::radians(80.0f),
            800.0f/600.0f,
            1.0f,
            10.0f
        );    
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f, 1.2f, 1.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    
    glEnable(GL_DEPTH_TEST);
    SDL_Event windowEvent;
    while(true) 
    {
        if(SDL_PollEvent(&windowEvent)) 
        {
            if (windowEvent.type == SDL_QUIT) {break;}
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) {break;}
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        GLint uniModel = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model,
            (SDL_GetTicks() / 1024.0f )* glm::radians(180.0f),
            glm::vec3(1.0, 0.0, 1.0)
        );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(2, textures);
    glDeleteVertexArrays(1, &vao);
    
    SDL_Quit();
    SDL_GL_DeleteContext(context);
}