set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_subdirectory(glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_BUILD_X11 ON)
set(GLFW_BUILD_WAYLAND OFF)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(spdlog)
# add_subdirectory(imgui)
file(GLOB IMGUI_SOURCE_FILES "imgui/*.cpp" "imgui/backends/imgui_impl_opengl3.cpp" "imgui/backends/imgui_impl_glfw.cpp" "imgui/misc/cpp/imgui_stdlib.cpp")
file(GLOB IMGUI_HEADERS "imgui/*.h" "imgui/backends/imgui_impl_opengl3.h" "imgui/backends/imgui_impl_glfw.h" "imgui/misc/cpp/imgui_stdlib.h" )

# make variables available for parent cmake file
set(IMGUI_SOURCE_FILES ${IMGUI_SOURCE_FILES} PARENT_SCOPE)
set(IMGUI_HEADERS ${IMGUI_HEADERS} PARENT_SCOPE)
