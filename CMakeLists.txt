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

        src/AssetManager.cpp
        src/Application.cpp
        src/callbacks.cpp
        src/Camera.cpp
        src/EventHandler.cpp
        src/Image.cpp
        src/SceneGraph.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Window.cpp

        # Culling Module
        src/culling/AABB.cpp
        src/culling/Frustum.cpp

        # Entities Module
        src/entities/DrawableEntity.cpp
        src/entities/Entity.cpp
        src/entities/FlatShadedMeshEntity.cpp
        src/entities/LineMeshEntity.cpp
        src/entities/ModelEntity.cpp
        src/entities/TerrainEntity.cpp
        src/entities/TriangleMeshEntity.cpp

        # Maths Module
        src/maths/functions.cpp
        src/maths/geometry.cpp
        src/maths/mat3.cpp
        src/maths/mat4.cpp
        src/maths/quaternion.cpp
        src/maths/Transform.cpp
        src/maths/transforms.cpp
        src/maths/trigonometry.cpp

        # Mesh Module
        src/mesh/LineMesh.cpp
        src/mesh/Material.cpp
        src/mesh/Mesh.cpp
        src/mesh/Model.cpp
        src/mesh/PointMesh.cpp
        src/mesh/primitives.cpp
        src/mesh/Terrain.cpp
        src/mesh/TriangleMesh.cpp

        # Utility Module
        src/utility/hash.cpp
        src/utility/LifetimeLogger.cpp
        src/utility/Random.cpp

        # Libraries
        lib/cgltf/cgltf.cpp

        lib/glad/src/glad.c

        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp

        lib/stb/stb_image.cpp
)

set(INCLUDES
        include

        # Libraries
        lib/glad/include
        lib/imgui
        lib/imgui/backends
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

# Creates a macro called 'DEBUG' when program is launched in debug mode
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:DEBUG>)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)