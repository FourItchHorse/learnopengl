#include <GLAD/glad.h> //starting from scratch without copying to revise concepts
#include <GLFW/glfw3.h>

#include <iostream>

//goal: create two triangles, one blue one yellow with different VAOs and VBOS (and two different fragment shaders)
GLFWwindow* createContextAndWindow(int width, int height, const char* title);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330\n"
"layout (location = 0) in vec3 aPos;\n" 
"void main()" 
"{" 
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"}";

const char* fragmentShaderBlueSource = "#version 330\n"
"out vec4 FragColor;"
"void main() "
"{ FragColor = vec4(0.0, 0.0, 0.7, 1.0);"
" }";

const char* fragmentShaderYellowSource = "#version 330\n"
"out vec4 FragColor;"
"void main() {"
" FragColor = vec4(0.7f, 0.7f, 0.0f, 1.0f);"
"}";

int main() 
{
    glfwInit();
    
    GLFWwindow* window = createContextAndWindow(800,600,"Two different colour triangles");

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderBlue = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int blueShader = glCreateProgram();
    unsigned int yellowShader = glCreateProgram();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
    glCompileShader(fragmentShaderYellow);
    glShaderSource(fragmentShaderBlue, 1, &fragmentShaderBlueSource, NULL);
    glCompileShader(fragmentShaderBlue);

    glAttachShader(yellowShader, vertexShader);
    glAttachShader(yellowShader, fragmentShaderYellow);
    glLinkProgram(yellowShader);
    
    glAttachShader(blueShader, vertexShader);
    glAttachShader(blueShader, fragmentShaderBlue);
    glLinkProgram(blueShader);

    float triangleY[] 
    {
        -0.25, 1.0, 0.0,
        -0.5, -0.5, 0.0, 
        0, -0.5, 0.0
    };
    float triangleB[]
    {
        0.75, 1.0, 0.0, 
        0.5, -0.5, 0.0,
        1.0, -0.5, 0.0f,
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleY), triangleY, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleB), triangleB, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2,0.2,0.2,1.0); //rendered in a different colour         
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(yellowShader);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(blueShader);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwTerminate();
    return 0;
}


GLFWwindow* createContextAndWindow(int width, int height, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(window == NULL) 
    {
        std::cout<< "FAILED TO CREATE GLFW WINDOW" <<std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout<<"FAILED TO INITIALIZE GLAD"<<std::endl;
        glfwTerminate();
        return nullptr;
    }

    glViewport(0,0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    return window;
}


void processInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0,0,width, height);
}