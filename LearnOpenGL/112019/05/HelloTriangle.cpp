#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n\nlayout (location = 0) in vec3 aPos;\n\nvoid main()\n{\n\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\n}";
const char* fragShaderSource = "#version 330 core\n\nout vec4 FragColor;\n\nvoid main()\n{\nFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window); 
unsigned int compileShaders(const char* vertexSource, const char* fragmentSource);
GLFWwindow*  createGLFWwindowAndContext(int width, int height, const char* name);


int main () 
{
    glfwInit();

    GLFWwindow* window = createGLFWwindowAndContext(800,600,"HelloTriangle");

    unsigned int shaderProgram = compileShaders(vertexShaderSource, fragShaderSource);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //rendering

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback (GLFWwindow* window, int width, int height) 
{
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int compileShaders(const char* vertexSource, const char* fragmentSource) 
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLFWwindow* createGLFWwindowAndContext(int width, int height, const char* name) 
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width,height,name, nullptr, nullptr);

    if (!window)
    {
        std::cout << "Failed to create glfw window." << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD>" << std::endl;
        glfwTerminate();
    }
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    return window;
}