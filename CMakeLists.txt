cmake_minimum_required(VERSION 3.17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(project_target stupid_swarm)

set(include_directories
    ext/glfw/include
    ext/glad/include
    ext/glm
    ext/spdlog/include
    src
)

set(libraries
    glad
    glfw
    glm::glm
    spdlog::spdlog
)

project(${project_target})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ext)

file(GLOB_RECURSE executable_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(stupid-swarm ${executable_sources})

target_compile_options(stupid-swarm PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(stupid-swarm PUBLIC ${include_directories})

target_link_libraries(stupid-swarm PUBLIC ${libraries})
