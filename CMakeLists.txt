cmake_minimum_required(VERSION 3.22.0)
project(raylib-template CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###################################################################################
# get external dependecies uisng git fetch content
###################################################################################
include(FetchContent)

function(fetch_git_dependency name url tag subdir compile_options)
    if ("${subdir}" STREQUAL "")
        set(subdir ".")
    endif()
    FetchContent_Declare(
        ${name}
        GIT_REPOSITORY ${url}
        GIT_TAG ${tag}
        SOURCE_SUBDIR ${subdir}
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(${name})
    if (${compile_options} STREQUAL "PRIVATE")
        target_compile_options(${name} PRIVATE -w) # Suppress warnings for external dependencies
    elseif (${compile_options} STREQUAL "PUBLIC")
        target_compile_options(${name} PUBLIC -w) # Suppress warnings for external dependencies
    elseif (${compile_options} STREQUAL "INTERFACE")
        target_compile_options(${name} INTERFACE -w) # Suppress warnings for external dependencies
    endif()
endfunction()

# Add raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
fetch_git_dependency(
    raylib
    https://github.com/raysan5/raylib.git
    5.5
    ""
    "PRIVATE"
)

# Add raygui
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
fetch_git_dependency(
    raygui
    https://github.com/raysan5/raygui.git
    4.0
    "projects/CMake"
    "INTERFACE"
)

# Add box2d
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
fetch_git_dependency(
    box2d
    https://github.com/erincatto/box2d.git
    v3.1.1
    ""
    "PRIVATE"
)

# Add fmt
fetch_git_dependency(
    fmt
    https://github.com/fmtlib/fmt.git
    11.2.0
    ""
    "PRIVATE"
)

# FetchContent directly due to ImGui and rlImGui not being CMake projects
# Add Dear-ImGui
FetchContent_Populate(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/imgui
)
add_library(imgui)
target_sources(imgui PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/build/imgui")
target_link_libraries(imgui PUBLIC raylib_static glfw)

# Add rlImGui
FetchContent_Populate(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/rlImGui
)
add_library(rlimgui)
target_sources(rlimgui PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/build/rlImGui/rlImGui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/rlImGui/rlImGui.h"
)
target_include_directories(rlimgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/build/rlImGui")
target_link_libraries(rlimgui PUBLIC imgui raylib_static)

###################################################################################
# Executable setup
###################################################################################
# Adding source files
# This sets SOURCE_FILES to all .cpp files in the src directory
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# This sets SOURCE_HEADERS to all .h files in the src directory
set(SOURCE_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raygui)
target_link_libraries(${PROJECT_NAME} PRIVATE box2d)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE rlimgui)

###################################################################################
# Build Settings
###################################################################################
# Set the build type to Debug or Release
add_definitions(-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -DEBUG")
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
endif()

# Set common compile options
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
