#define GLEW_STATIC
#include <GL/glew.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <iostream>

//create vertex and fragment shader source code
const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord1;
    in vec2 texcoord2;
    out vec3 Color;
    out vec2 Texcoord1;
    out vec2 Texcoord2;
    uniform mat4 transform;

    void main() 
    {
        Color = color;
        Texcoord1 = texcoord1;
        Texcoord2 = texcoord2;

        gl_Position = transform * vec4(position.x, position.y, 0.0, 1.0);
    }

)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    uniform sampler2D tex1;
    uniform sampler2D tex2;
    uniform float texWeight;
    uniform mat4 transform;
    in vec3 Color;
    in vec2 Texcoord1;
    in vec2 Texcoord2;
    out vec4 outColor;

    void main() 
    {
        vec4 colTex1 = texture(tex1, Texcoord1);
        vec4 colTex2 = texture(tex2, Texcoord2);
        outColor = mix(colTex1, colTex2, 0.5) * (transform * vec4(Color, 1));
    }

)glsl";

int main() 
{
    //Init, set up SDL_GL settings, window and context 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    

    SDL_Window* window = SDL_CreateWindow("Transformations with GLM", SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    

    glewExperimental = GL_TRUE;
    glewInit();
    printf("INITIALISING: OPENGL ERROR: %u\n", glGetError());

    //create shader ints and compile/link shaders and program 
    GLint status;
    GLchar buffer[512];
    
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout << "VERTEX SHADER COMPILE STATUS: " << status <<" VERTEX SHADER INFO LOG:\n" << buffer;
    printf("VERTEX SHADER OPENGL ERROR: %u\n", glGetError());
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    std::cout << "FRAGMENT SHADER COMPILE STATUS: " << status << " FRAGMENT SHADER INFO LOG:\n" << buffer;
    printf("FRAGMENT SHADER OPENGL ERROR: %u\n", glGetError());
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    std::cout << "SHADER PROGRAM LINK STATUS: " << status << " SHADER PROGRAM INFO LOG:\n" << buffer;
    glUseProgram(shaderProgram);
    printf("SHADER PROGRAM OPENGL ERROR: %u\n", glGetError());
    
    //create vertex array objects, vertex buffer objects, element buffer objects, texture objects and data for all
    GLuint vao, vbo, ebo, textures[2];
    glGenVertexArrays(1, &vao);
    //create data for objects
    GLfloat vertices[] {
        //Position      Color      TexCoords1   TexCoords2
        -0.5, 0.5,  1.0, 0.0, 0.0,  0.0, 0.0,   0.0, 0.0,
         0.5, 0.5,  1.0, 1.0, 0.0,  1.0, 0.0,   5.0, 0.0,
        0.5, -0.5,  1.0, 1.0, 1.0,  1.0, 1.0,   5.0, 10.0,
        -0.5, -0.5, 1.0, 1.0, 0.0,  0.0, 1.0,   0.0, 10.0
    };

    GLuint indices[]
    {
        0, 1, 3,
        1, 2, 3
    };
    
    //bind and add data to array and buffer objects. Create and add data to texture objects
    glBindVertexArray(vao);
    
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    printf("WRITNG TO VBO OPENGL ERROR: %u\n", glGetError());
    

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    printf("WRITING TO EBO OPENGL ERROR: %u\n", glGetError());
    
    
    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    printf("BINDING TO TEXTURE1 OPENGL ERROR: %u\n", glGetError());
    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    printf("SETTING TEXTURE1 OPENGL ERROR: %u\n", glGetError());
    
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
    printf("WRITING TO TEXTURE1 OPENGL ERROR: %u\n", glGetError());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    printf("SETTING UP TEXTURE2 OPENGL ERROR: %u\n", glGetError());
    
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
    printf("WRITING TO TEXTURE2 OPENGL ERROR: %u\n", glGetError());
    
    //create and enable vertexAttrib for position, color and texcoords ("in" values of shader program)
    
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
    
    //render loop

    printf("OPENGL ERROR: %u\n", glGetError());
    SDL_Event windowEvent;
    while (true)
    {
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
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
 

        float time = SDL_GetTicks()/ 1024.0f;
        GLint uniTransform = glGetUniformLocation(shaderProgram, "transform");
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(
        transform, 
        time * glm::radians(90.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f)
        );

        glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(transform));
        
        
        printf("TIME: %f\r", time);
        glUniform1f(glGetUniformLocation(shaderProgram, "texWeight"), 0.5);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0); 
        glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
   
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    //quit and delete context
    
    SDL_Quit();
    SDL_GL_DeleteContext(context);
    return 0;
}