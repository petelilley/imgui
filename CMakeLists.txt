cmake_minimum_required(VERSION 3.22)

# Modified CMakeLists.txt from https://github.com/adobe/imgui

project(imgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(IMGUI_DEFINE_MATH_OPERATORS "Enable ImGui ImVec2/ImVec4 operators" OFF)
option(IMGUI_ENABLE_STDLIB "Enable imgui_stdlib header" ON)
set(IMGUI_BACKEND_RENDERER "detect" CACHE STRING "Which renderer backend to use?")
set_property(CACHE IMGUI_BACKEND_RENDERER PROPERTY STRINGS detect none allegro5 dx9 dx10 dx11 dx12 metal opengl2 opengl3 sdlrenderer2 sdlrenderer3 vulkan wgpu)
set(IMGUI_BACKEND_PLATFORM "detect" CACHE STRING "Which platform backend to use?")
set_property(CACHE IMGUI_BACKEND_PLATFORM PROPERTY STRINGS detect none allegro5 android glfw glut osx sdl2 sdl3 win32)

add_library(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_raii.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"
)

if(IMGUI_ENABLE_STDLIB)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")
endif()

if(IMGUI_DEFINE_MATH_OPERATORS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DIMGUI_IMPL_OPENGL_LOADER_GL3W=1
    -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS
    # -DImTextureID=void*
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                                  "${CMAKE_CURRENT_SOURCE_DIR}/backends")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# ###########
# BACKENDS #
# ###########

if(IMGUI_BACKEND_RENDERER STREQUAL "detect")
    message(FATAL_ERROR "Cannot detect renderer backend. Please set IMGUI_BACKEND_RENDERER to a specific backend.")
endif()
if(IMGUI_BACKEND_PLATFORM STREQUAL "detect")
    set(IMGUI_BACKEND_PLATFORM "glfw")
endif()

if(IMGUI_BACKEND_RENDERER STREQUAL "none")
# do nothing. imgui will compile without a renderer.
elseif(IMGUI_BACKEND_RENDERER STREQUAL "allegro5")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_allegro5.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_allegro5.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx9")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx9.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx9.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d9)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx10")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx10.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx10.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d10)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx11")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx11.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx11.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 d3dcompiler dwmapi)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx12")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx12.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx12.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d12 dxgi)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "metal")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_metal.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_metal.mm")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Metal -framework QuartzCore")
elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl2")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl2.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp")

    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL)
    elseif(TARGET OpenGL::GL)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
    endif()
elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl3")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL)
    elseif(TARGET OpenGL::GL)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
    endif()
elseif(IMGUI_BACKEND_RENDERER STREQUAL "sdlrenderer2")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "sdlrenderer3")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "vulkan")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_vulkan.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp")
    # This likely requires you to install the Vulkan SDK.
    find_package(Vulkan REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
    # Untested.
elseif(IMGUI_BACKEND_RENDERER STREQUAL "wgpu")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_wgpu.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp")
    # untested, needs work
endif()

if(IMGUI_BACKEND_PLATFORM STREQUAL "none")
    # do nothing. imgui will compile without a platform.
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "allegro5")
    # already included by the renderer backend above
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "android")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/android.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/android.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "glfw")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")

    if(NOT TARGET glfw)
        include(FetchContent)
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG tags/3.4
            GIT_SHALLOW TRUE
        )

        option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
        option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
        option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
        option(GLFW_INSTALL "Generate installation target" OFF)
        option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" OFF)
        FetchContent_MakeAvailable(glfw)
    endif()

    target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "glut")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glut.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glut.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "osx")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_osx.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_osx.mm")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework AppKit -framework MetalKit -framework Foundation -framework GameController")
    # This builds successfully, but hangs on startup. Needs work.
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "sdl2")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdl2.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "sdl3")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdl3.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "win32")
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_win32.h"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_win32.cpp")
endif()

