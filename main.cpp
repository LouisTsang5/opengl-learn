#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

#include "lib/app.h"
#include "lib/constant.h"

#define WIDTH 800
#define HEIGHT 600

#define VERTEX_SHADER_SOURCE_FILE "shaders/vertex.glsl"
#define FRAGMENT_SHADER_SOURCE_FILE "shaders/frag.glsl"

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

std::string read_file(const std::string_view file_name)
{
    // Check for size
    auto size = std::filesystem::file_size(file_name);

    // Reserve buffer
    std::string retval;
    retval.reserve(size);

    // Read file
    std::ifstream in(file_name.data());
    in.read(retval.data(), size);

    return retval;
}

int main()
{
    puts("Starting...");

    // Read shaders
    puts("Reading shaders...");
    const std::string v_shader = read_file(VERTEX_SHADER_SOURCE_FILE);
    const char *const v_shaders[] = {v_shader.c_str()};
    const std::string f_shader = read_file(FRAGMENT_SHADER_SOURCE_FILE);
    const char *const f_shaders[] = {f_shader.c_str()};

    // Initialize app
    puts("Initializing app...");
    App app(WIDTH, HEIGHT, WIN_TITLE);
    app.use_vertices(VERTICES, ELEMENTS);
    app.use_shaders(v_shaders, f_shaders);
    auto window = app.window();

    // Main loop
    puts("Running...");
    while (!app.done())
    {
        app.update();
    }

    puts("Closing...");
    return 0;
}