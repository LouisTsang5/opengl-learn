#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define APP_GLFW_CTX_VER_MAJOR 3
#define APP_GLFW_CTX_VER_MINOR 3
#define SHADER_ERR_LOG_LENGTH 512
#define WIDTH 800
#define HEIGHT 600

typedef struct
{
    float x;
    float y;
    float z;
} Vertex;

const char *VertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

GLFWwindow *init();
void process_input(GLFWwindow *window);
unsigned int make_shader(GLenum shader_type, GLsizei count, const GLchar **shaderSource);

int main()
{
    puts("Starting...");
    GLFWwindow *window = init();

    // define vertices
    Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f},
    };

    // Make buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Make Vertex Shader
    if (!make_shader(GL_VERTEX_SHADER, 1, &VertexShaderSource))
    {
        goto exit_err;
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate
    glfwTerminate();
    return 0;

exit_err:
    glfwTerminate();
    return -1;
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int make_shader(GLenum shader_type, GLsizei count, const GLchar **shaderSource)
{
    // Create shader
    unsigned int shader = glCreateShader(shader_type);
    if (!shader)
    {
        puts("Create shader failed");
        return 0;
    }

    // Compile
    glShaderSource(shader, count, shaderSource, NULL);
    glCompileShader(shader);

    // Check for error
    int success;
    char infoLog[SHADER_ERR_LOG_LENGTH];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, SHADER_ERR_LOG_LENGTH, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        return 0;
    }

    // Return shader
    return shader;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("New dimensions - width: %d, height: %d\n", width, height);
    glViewport(0, 0, width, height);
}

GLFWwindow *init()
{
    // Init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, APP_GLFW_CTX_VER_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, APP_GLFW_CTX_VER_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        puts("Failed to create GLFW window");
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        puts("Failed to initialize GLAD");
        return NULL;
    }

    // Set view port
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}