# Global Project Configuration
cmake_minimum_required(VERSION 3.26)
project(stage-l3)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Compiler options
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Set sources and includes
set(SOURCES
        src/main.cpp

        src/Application.cpp
        src/callbacks.cpp
        src/Camera.cpp
        src/EventHandler.cpp
        src/Shader.cpp
        src/Window.cpp

        # Maths Module
        src/maths/geometry.cpp
        src/maths/ivec3.cpp
        src/maths/mat3.cpp
        src/maths/mat4.cpp
        src/maths/transforms.cpp
        src/maths/trigonometry.cpp
        src/maths/vec2.cpp
        src/maths/vec3.cpp
        src/maths/vec4.cpp

        # Mesh Module
        src/mesh/LineMesh.cpp
        src/mesh/Material.cpp
        src/mesh/Mesh.cpp
        src/mesh/Model.cpp
        src/mesh/PointMesh.cpp
        src/mesh/TriangleMesh.cpp

        # Utility Module
        src/utility/hash.cpp

        # Libraries
        lib/glad/src/glad.c
        lib/stb/stb_image.cpp
)

set(INCLUDES
        include

        # Libraries
        lib/glad/include
        lib/stb
)

set(LIBRARIES
        glfw
        dl
        pthread
        X11
        Xxf86vm
        Xrandr
        Xi
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)