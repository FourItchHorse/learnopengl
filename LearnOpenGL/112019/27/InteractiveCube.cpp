
#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include <SDL2/SDL.h>
#include <SDL2\SDL_opengl.h>

#include <STB/stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc\matrix_transform.hpp>
#include <glm/gtc\type_ptr.hpp>

#include <iostream>
#include <vector>

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;



glm::mat4 modelTransform(float mouseX, float mouseY, float cursorDown, float cursorRight, float s, float d); 

glm::mat4 cameraTransform (float mouseX, float mouseY, float cursorDown, float cursorRight, float s, float d); 

glm::mat4 projectionTransfrom(bool isPerspective, float right);

glm::mat4 generateCube (glm::vec3 position, int spaceCount, GLuint shaderProgram); 


const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    
    in vec3 position;
    in vec3 color;
    in vec2 texcoord1;

    out vec3 Color;
    out vec2 Texcoord1;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main () 
    {
         Color = color;
        Texcoord1 = texcoord1;

        gl_Position = proj * view * model * vec4(position, 1.0);
    }
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
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

/*Goal: create cohesive code for an interactive scene with a single cube */


int main () 
{
    SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_Window* window = SDL_CreateWindow("Interactive cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WND_WIDTH, WND_HEIGHT, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();

    printf("INITIALISING ERROR: %i\n", glGetError());

    GLint status;
    GLchar buffer[512];
    GLuint vertexShader, fragmentShader, shaderProgram;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        std::cout<<buffer << std::endl;        
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        std::cout<<buffer << std::endl;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
        glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
        std::cout<<buffer << std::endl;
    glUseProgram(shaderProgram);

    
    printf("SHADERS ERROR: %i\n", glGetError());

    GLuint vao, vbo, ebo, textures[2];
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glGenTextures(2, textures);
        GLfloat vertices[] 
    {
    
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 
};
    
        GLuint indices[] 
        {
            0, 1, 3,
            1, 2, 3,
            4, 5, 7,
            5, 6, 7, 
            0, 11, 7,

        }; 
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        /*
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        */

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
    
        data = stbi_load("Textures/cat.png", &width, &height, &nrChannels, 0);
        if(data) 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else 
        {
            std::cout << "FAILED TO LOAD TEXTURE 2!" << std::endl;
        }
    stbi_image_free(data);
    
    
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
        glVertexAttribPointer(texAttrib1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));
     printf("TEXCOORD1 ATTRIB OPENGL ERROR: %u\n", glGetError());
    glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
    

    glEnable(GL_DEPTH_TEST);
    SDL_Event windowEvent;
    const uint8_t * keyboardState = SDL_GetKeyboardState(0);
    int x, y, spaceCount = 0;
    float s, d, down, right, rightBracket, randomX, randomY, randomZ;
    rightBracket = 10.0f;
    bool isPerspective = true;
    float increment = 1.0f /32.0f;
    while (true)
    {
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
    
        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym  == SDLK_SPACE) {
            std::cout << "Generated cube!\n";
            spaceCount++;
            randomX = rand()%2;
            randomY = rand()%2;
            randomZ = rand()%14;
            printf("%f, %f, %f\n", randomX, randomY, randomZ);
            }
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            if(keyboardState[SDL_SCANCODE_ESCAPE]) {
            break;}

            if(windowEvent.type == SDL_QUIT) {
            break;}

            if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym  == SDLK_b) {
            std::cout << "Changed Perspective!\n";
            isPerspective = !isPerspective;
            }

            if(keyboardState[SDL_SCANCODE_UP]) {
            down -= increment;}

            if(keyboardState[SDL_SCANCODE_DOWN]){
            down += increment;}

            if(keyboardState[SDL_SCANCODE_LEFT]) {
            right -= increment;}

            if(keyboardState[SDL_SCANCODE_RIGHT]){
            right += increment;}

            if(keyboardState[SDL_SCANCODE_RIGHTBRACKET]){
            rightBracket += increment;}
            
            if(keyboardState[SDL_SCANCODE_LEFTBRACKET]){
            rightBracket -= increment;}
        }
        SDL_GetMouseState(&x, &y);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        

        glm::mat4 proj = glm::mat4(1.0f);
        proj = projectionTransfrom(isPerspective, rightBracket);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        
        //glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 view = cameraTransform(static_cast<float>(x), static_cast<float>(y), down, right, s, d);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        
        //glm::mat4 model = glm::mat4(1.0f);

        std::vector<glm::vec3> myCubes; 
        glm::mat4 model = glm::mat4(1.0f);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        for (int i = 0; i < spaceCount; i ++) {   
         glm::vec3 randomCubePos = glm::vec3(randomX, randomY, randomZ);
         model = generateCube(randomCubePos, spaceCount, shaderProgram);
         glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        /*
        for () {
        glm::mat4 model = glm::mat4(1.0f);
            if (spaceCount > 0) {
            model = glm::translate(model, myCubes[spaceCount]);
            float angle = 20.0f * spaceCount; 
            } else 
            {
                model = glm::translate(model, glm::vec3(0.0f));
            }
        } */

        glm::mat4 transform = modelTransform(static_cast<float>(x), static_cast<float>(y), down, right, s, d);
        model *= transform;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        SDL_GL_SwapWindow(window);

    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);    

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


glm::mat4 modelTransform(float mouseX, float mouseY, float cursorDown, float cursorRight, float s, float d) 
{
    mouseX  -= 400.0f;
    mouseX /= 400.0f;
    mouseY -= 300.0f;
    mouseY /= 300.0f;
    glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model,
            -cursorDown * glm::radians(55.0f),
            glm::vec3(mouseX * 0.25, -mouseY * 0.25, 0.0)
        );
    return model;
}

glm::mat4 projectionTransfrom(bool isPerspective, float right)
{
    glm::mat4 proj = glm::mat4(1.0f);
    if(isPerspective) 
    {
        proj = glm::perspective(
            glm::radians(45.0f * right),
            static_cast<float>(WND_WIDTH)/static_cast<float>(WND_HEIGHT),
            1.0f,
            100.0f
        );
    }
    else if (!isPerspective) 
    {
       proj = glm::perspective(
            glm::radians(1000.0f),
            static_cast<float>(WND_WIDTH)/static_cast<float>(WND_HEIGHT),
            -100.0f,
            1000.0f );
    }
    return proj;
}

glm::mat4 cameraTransform (float mouseX, float mouseY, float cursorDown, float cursorRight, float s, float d) 
{
    glm::mat4 view = glm::lookAt(
        glm::vec3(cos(cursorRight), sin(cursorRight), 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );  
    return view;
}

glm::mat4 generateCube (glm::vec3 position, int spaceCount, GLuint shaderProgram) 
{
            glm::mat4 model = glm::mat4(1.0f);
            if (spaceCount > 0) {
            model = glm::translate(model, position);
            float angle = 20.0f * spaceCount; 
            } else 
            {
                model = glm::translate(model, glm::vec3(0.0f));
            }
            float angle = 20.0f * spaceCount;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            return model;
} 
 