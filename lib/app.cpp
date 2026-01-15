#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <format>
#include <memory>
#include <span>
#include <string_view>
#include <stdexcept>

#include "app.h"
#include "constant.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("New dimensions - width: %d, height: %d\n", width, height);
    glViewport(0, 0, width, height);
}

App::App(int width, int height, const std::string_view title)
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
    GLFWwindow *window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
    if (window == NULL)
        throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    // Set view port
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set member
    m_window = window;
}

App::~App()
{
    glfwTerminate();
}

unsigned int make_shader(GLenum shader_type, const std::span<const char *const> source)
{
    // Create shader
    unsigned int shader = glCreateShader(shader_type);
    if (!shader)
        throw std::runtime_error("Shader creation failed");

    // Compile
    glShaderSource(shader, source.size(), source.data(), NULL);
    glCompileShader(shader);

    // Check for error
    int success;
    char inner_log[GL_STACK_ERR_BUF_LEN];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(inner_log), NULL, inner_log);
        throw std::runtime_error(std::format("Shader compilation failed\n{}", inner_log));
    }

    // Return shader
    return shader;
}

void App::use_shaders(const std::span<const char *const> v_info, const std::span<const char *const> f_info)
{
    // Make Vertex Shader
    unsigned int v_shader = make_shader(GL_VERTEX_SHADER, v_info);

    // Make Fragment Shader
    unsigned int f_shader = make_shader(GL_FRAGMENT_SHADER, f_info);

    // Create Shader Program
    m_shader_prog = glCreateProgram();
    if (!m_shader_prog)
        throw std::runtime_error("Failed to create shader program");

    // Link shader programs
    glAttachShader(m_shader_prog, v_shader);
    glAttachShader(m_shader_prog, f_shader);
    glLinkProgram(m_shader_prog);
    int success;
    char inner_log[GL_STACK_ERR_BUF_LEN];
    glGetProgramiv(m_shader_prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shader_prog, sizeof(inner_log), NULL, inner_log);
        throw std::runtime_error(std::format("Shader program linking failed\n{}", inner_log));
    }

    // Delete shaders
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    // Use prog
    glUseProgram(m_shader_prog);
}

int App::uniform_location(const char *key) noexcept
{
    return glGetUniformLocation(m_shader_prog, key);
}

void App::use_vertices(const std::span<const Vertex> vertices, const std::span<const unsigned int> elements) noexcept
{
    // Set member var
    m_element_size = elements.size();

    // Make buffer
    glGenVertexArrays(1, &m_va_id);
    glBindVertexArray(m_va_id);

    // Bind and set buffer
    unsigned int vb_id;
    glGenBuffers(1, &vb_id);
    glBindBuffer(GL_ARRAY_BUFFER, vb_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Bind and set element buffer
    unsigned int eb_id;
    glGenBuffers(1, &eb_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

    // Set attribute pointer
    glVertexAttribPointer(0, N_VERTEX_COMPONENT, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
}

bool App::done() noexcept
{
    return glfwWindowShouldClose(m_window);
}

void App::update() noexcept
{
    // Handle escape key press
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    // Background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set color offset according to time
    float color_offset = ((float)sin(glfwGetTime()) + 1.f) / 2.f; // offset between 0. to 1.
    float pos_offset = (color_offset / 2.f) - 0.25f;              // offset between -0.25 to 0.25
    glUniform1f(uniform_location("colorOffset"), color_offset);
    glUniform1f(uniform_location("posOffset"), pos_offset);

    // render vertex
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, m_element_size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}