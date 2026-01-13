#include <GLFW/glfw3.h>
#include <span>
#include <string_view>

typedef struct
{
    float x;
    float y;
    float z;
} Vertex;
#define N_VERTEX_COMPONENT 3

class App
{
private:
    GLFWwindow *m_window;
    unsigned int m_shader_prog;
    unsigned int m_va_id;
    int m_element_size;

public:
    App(int width, int height, const std::string_view title);
    ~App();

    void use_shaders(const std::span<const char *const> v_info, const std::span<const char *const> f_info);
    void use_vertices(const std::span<const Vertex> vertices, const std::span<const unsigned int> elements) noexcept;
    void update() noexcept;

    [[nodiscard]] int uniform_location(const char *key) noexcept;

    [[nodiscard]] constexpr GLFWwindow *window() noexcept
    {
        return m_window;
    }

    [[nodiscard]] constexpr unsigned int shader_prog() noexcept
    {
        return m_shader_prog;
    }

    [[nodiscard]] bool done() noexcept;
};