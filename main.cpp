#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

#include "lib/app.h"
#include "lib/constant.h"

#define WIDTH 800
#define HEIGHT 600

constexpr const char *VertexShaderSource = "#version 330 core\n"
                                           "layout (location = 0) in vec3 aPos;\n"
                                           "uniform float colorOffset;"
                                           "out vec4 vertexColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   gl_Position = vec4(aPos, 1.0);\n"
                                           "   vertexColor = vec4((aPos.x * 2 + 1 + colorOffset) / 3, (aPos.y * 2 + 1 + colorOffset) / 3, (aPos.z * 2 + 1 + colorOffset) / 3, 1.0);"
                                           "}\0";

constexpr const char *FragmentShaderSource = "#version 330 core\n"
                                             "out vec4 FragColor;\n"
                                             "in vec4 vertexColor;\n"
                                             "void main()\n"
                                             "{FragColor = vertexColor;}\0";

// define vertices & elements
constexpr std::array<const Vertex, 4> VERTICES = {
    Vertex{-0.5f, 0.5f, 0.0f},
    Vertex{0.5f, 0.5f, 0.0f},
    Vertex{-0.5f, -0.5f, 0.0f},
    Vertex{0.5f, -0.5f, 0.0f},
};
constexpr std::array<const unsigned int, 6> ELEMENTS = {
    0,
    1,
    2,

    1,
    2,
    3,
};

// Define shaders
constexpr std::array<const char *, 1> V_SHADERS{VertexShaderSource};
constexpr std::array<const char *, 1> F_SHADERS{FragmentShaderSource};

int main()
{
    puts("Starting...");

    // Initialize app
    App app(WIDTH, HEIGHT, WIN_TITLE);
    app.use_vertices(VERTICES, ELEMENTS);
    app.use_shaders(V_SHADERS, F_SHADERS);
    auto window = app.window();

    // Main loop
    while (!app.done())
    {
        app.update();
    }

    return 0;
}