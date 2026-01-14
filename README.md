# Source

https://learnopengl.com/

# Compilation

Make sure GLFW is installed.

```bash
clang++ -std=c++20 main.cpp lib/glad.c lib/app.cpp\
    -I./include \
    -lglfw
```