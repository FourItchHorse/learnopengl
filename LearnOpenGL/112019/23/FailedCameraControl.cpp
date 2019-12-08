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

const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord1;
    in vec2 texcoord2;

    out vec3 Color;
    out vec2 Texcoord1;
    out vec2 Texcoord2;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() 
    {
        Color = color;
        Texcoord1 = texcoord1;
        Texcoord2 = texcoord2;

        gl_Position =  proj * view *  model * vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 Color;
    in vec2 Texcoord1;
    in vec2 Texcoord2;

    out vec4 outColor;

    uniform float time;
    uniform mat4 colorEffect;
    uniform mat4 texEffect;

    uniform sampler2D tex1;
    uniform sampler2D tex2;

    void main () 
    {
        vec4 colTex1 = texture(tex1, Texcoord1);
        vec4 colTex2 = texture(tex2, Texcoord2 + time);
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

        SDL_Window* window = SDL_CreateWindow("Interactive Star", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
            std::cout << buffer << std::endl;

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
            std::cout << buffer << std::endl;

                glUseProgram(shaderProgram);

    GLuint vbo, ebo, vao, textures[2];
    glGenVertexArrays(1, &vao);
    GLfloat vertices[] 
    {
         0.0, -0.75,    1.0, 0.0, 0.0,    0.5, -1.0,     2.5,-10.0,    
       -0.15, -0.25,    0.0, 1.0, 0.0,   -0.1,  0.0,     0.0,  0.0,
        0.15, -0.25,    0.0, 0.0, 1.0,    0.1,  0.0,    3.75,  0.0,
       -0.65, -0.25,    0.0, 1.0, 0.0,   -1.0,  0.0,    -5.0,  0.0, 
        0.65, -0.25,    1.0, 0.0, 0.0,    1.0,  0.0,     5.0,  0.0, 
       -0.20,   0.0,    0.0, 1.0, 0.0,  -0.15, 0.25,    -1.0,  2.5, 
        0.20,   0.0,    0.0, 0.0, 1.0,   0.15, 0.25,     1.0,  2.5,
         0.0,  0.15,    0.0, 1.0, 0.0,    0.5, 0.45,     2.5,  5.0,   
       -0.45,   0.5,    1.0, 0.0, 0.0,   -1.0,  1.0,    -5.0, 10.0,
        0.45,   0.5,    0.0, 1.0, 0.0,    1.0,  1.0,     5.0, 10.0,
    };
    GLuint indices[]
    {
        1, 0, 2,    3, 1, 5,
        7, 5, 1,    1, 2, 7, 
        4, 2, 6,    7, 6, 2, 
        5, 8, 7,    7, 9, 6,
    };

    glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) 0);
    printf("POSITION ATTRIB OPENGL ERROR: %u\n", glGetError());
    

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    printf("COLOR ATTRIB OPENGL ERROR: %u\n", glGetError());

    
    GLint texAttrib1 = glGetAttribLocation(shaderProgram, "texcoord1");
    glEnableVertexAttribArray(texAttrib1);
    glVertexAttribPointer(texAttrib1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    printf("TEXCOORD1 ATTRIB OPENGL ERROR: %u\n", glGetError());
    
    
    GLint texAttrib2 = glGetAttribLocation(shaderProgram, "texcoord2");
    glEnableVertexAttribArray(texAttrib2);
    glVertexAttribPointer(texAttrib2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (7 * sizeof(GLfloat)));
    printf("TEXCOORD2 ATTRIB OPENGL ERROR: %u\n", glGetError()); 
    
    //set non-live uniforms
    //render loop
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), 
        800.0f/ 600.0f,
        1.0f,
        5.0f
        );
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glm::mat4 view = glm::lookAt(
            glm::vec3(1.2f, 1.2f, 1.2f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        ); 
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
        


    SDL_Event windowEvent;
    while (true)
    { 
        /* NOTES: for rotating star on the x-axis with mousey multiply
        the static float cast of mouse y by 0.15 for a rotation between 0 
        and 90 and 0.0075 for a rotation between 0 and 90. Maybe change 
        projection matrix angle within the render loop and control with mousey?

        For rotating the star about the z axis with mouseX: find a way to 
        offset the mouse so that 400 is the centre(0), 0 is the leftmost (-1)
        and 800 is the rightmost(1). Maybe (mousex - 400)/400?

        Ok, so to make sure the camera focuses on the star maybe also manipulate the
        view matrix's z coordinate within the loop with mouse y as well
        */
        int x, y;
        if(SDL_PollEvent(&windowEvent)) 
        {
            if (windowEvent.type == SDL_QUIT) {break;}
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) {break;}
            SDL_GetMouseState(&x, &y);
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        float mouseX, mouseY;
        mouseX = static_cast<float>(x);
        mouseY = static_cast<float>(y);
        float mouseRotation = (mouseX - 400.0f)/400.0f;
        float mouseCameraAngle = (600 - mouseY) * 45.0f/600.0f;
        float mouseCameraLookAt = (600 - mouseY) * 5.0f/600.0f;

        glUniform1f(glGetUniformLocation(shaderProgram, "time"), mouseCameraAngle);
        
        glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);


        GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    
        glUniformMatrix4fv(uniProj, 1, GL_TRUE, glm::value_ptr(proj));

        
        GLint uniView = glGetUniformLocation(shaderProgram, "view");
        glm::mat4 view = glm::lookAt(
            glm::vec3(1.2f, (1.2f * mouseCameraLookAt), (1.2f * mouseCameraLookAt)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glUniformMatrix4fv(uniView, 1, GL_TRUE, glm::value_ptr(view));


        GLint uniModel = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model,
            mouseRotation * glm::radians(360.0f),
            glm::vec3(0.0, 0.0, 1.0)
        );
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        printf("MOUSE X: %f \t MOUSE Y: %f \t MODEL ROTATION SCALAR: %f \t CAMERA ANGLE: %f \t CAMERA Z POS: %f \r", 
        mouseX, mouseY, mouseRotation, mouseCameraAngle, mouseCameraLookAt);

        
        glBindVertexArray(vao);
    
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);


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