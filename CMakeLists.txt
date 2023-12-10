# Minimum Version of CMake Required to Generate Project Build Files
cmake_minimum_required(VERSION 3.27)

# Create Project and Set Programming Config
project(learningOpenGL C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -Wall -Wextra -pedantic-errors")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Set Project's Include and Library Directory
include_directories(${PROJECT_NAME} "include/")
link_directories(${PROJECT_NAME} "lib/linux/")

# Link Source Files to Project
add_executable(${PROJECT_NAME}
    # Main Source File
    "src/main.cpp"

    # Custom Engine Source
    "src/Engine/Shader/shader.cpp"
    "src/Engine/Camera/camera.cpp"
    "src/Engine/Texture/texture.cpp"
    "src/Engine/Framebuffer/framebuffer.cpp"
    "src/Engine/Model/mesh.cpp"
    "src/Engine/Model/model.cpp"

    # GLAD OpenGL Loader
    "src/glad.c"
)

# Project Linkers
target_link_libraries(${PROJECT_NAME}
    glfw3 wayland-client assimp
)
