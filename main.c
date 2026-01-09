#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define APP_GLFW_CTX_VER_MAJOR 3
#define APP_GLFW_CTX_VER_MINOR 3
#define WIDTH 800
#define HEIGHT 600

// Define global error log
#define GBL_ERR_LOG_LENGTH 1024
char gbl_err_log[GBL_ERR_LOG_LENGTH];
#define logerr(...) snprintf(gbl_err_log, sizeof(gbl_err_log), __VA_ARGS__);

typedef struct
{
    float x;
    float y;
    float z;
} Vertex;

typedef struct
{
    const GLsizei count;
    const GLchar **shaderSource;
} ShaderInfo;

const char *VertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *FragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}\0";

GLFWwindow *init();
unsigned int make_shader_prog(const ShaderInfo *v_info, const ShaderInfo *f_info);
void process_input(GLFWwindow *window);

int main()
{
    puts("Starting...");
    GLFWwindow *window = init();
    if (!window)
        goto exit_err;

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

    // Define shaders
    const ShaderInfo v_shader = {1, &VertexShaderSource};
    const ShaderInfo f_shader = {1, &FragmentShaderSource};

    // Make shader program
    const unsigned int shader_prog = make_shader_prog(&v_shader, &f_shader);
    if (!shader_prog)
        goto exit_err;
    glUseProgram(shader_prog);

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
    puts(gbl_err_log);
    glfwTerminate();
    return -1;
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int make_shader(GLenum shader_type, const ShaderInfo *info)
{
    // Create shader
    unsigned int shader = glCreateShader(shader_type);
    if (!shader)
    {
        logerr("Shader creation failed");
        return 0;
    }

    // Compile
    glShaderSource(shader, info->count, info->shaderSource, NULL);
    glCompileShader(shader);

    // Check for error
    int success;
    char inner_log[GBL_ERR_LOG_LENGTH];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(inner_log), NULL, inner_log);
        logerr("Shader compilation failed\n%s", inner_log);
        return 0;
    }

    // Return shader
    return shader;
}

unsigned int make_shader_prog(const ShaderInfo *v_info, const ShaderInfo *f_info)
{
    // Make Vertex Shader
    unsigned int v_shader = make_shader(GL_VERTEX_SHADER, v_info);
    if (!v_shader)
        return 0;

    // Make Fragment Shader
    unsigned int f_shader = make_shader(GL_FRAGMENT_SHADER, f_info);
    if (!f_shader)
        return 0;

    // Create Shader Program
    unsigned int shader_prog = glCreateProgram();
    if (!shader_prog)
    {
        logerr("Failed to create shader program");
        return 0;
    }

    // Link shader programs
    glAttachShader(shader_prog, v_shader);
    glAttachShader(shader_prog, f_shader);
    glLinkProgram(shader_prog);
    int success;
    char inner_log[GBL_ERR_LOG_LENGTH];
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_prog, sizeof(inner_log), NULL, inner_log);
        logerr("Shader program linking failed\n%s", inner_log);
        return 0;
    }

    // Delete shaders
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    // Return program
    return shader_prog;
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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Make window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        logerr("Failed to create GLFW window");
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logerr("Failed to initialize GLAD");
        return NULL;
    }

    // Set view port
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}