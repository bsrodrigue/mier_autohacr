cmake_minimum_required(VERSION 3.10)  # Set the minimum required version of CMake

# Project name and language
project(autohacka LANGUAGES CXX)

# Define directories
set(SRC_DIR src)
set(BUILD_DIR build)

# Find all source files
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cc)
file(GLOB_RECURSE HEADER_FILES ${SRC_DIR}/*.h)

# Specify the executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Set the include directories if needed
# target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# Link against required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib m stdc++)

# Set compiler flags
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_OPTIONS "-Wall;-Wextra;--debug"
)

# Specify the output directory for binaries
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Custom clean command
set_directory_properties(PROPERTIES
    ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)

