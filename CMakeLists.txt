
# 1. Define the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Set the project name and language
project(StudentManagementSystem LANGUAGES C)

# 3. Specify the C standard (C99 is standard for first-year projects)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 4. Add the executable and list the source files
# This tells CMake to combine main.c and functions.c into one app
add_executable(student_app main.c functions.c)

# 5. (Optional) Tell CMake where to find the header files 
# Since student.h is in the same folder, this is usually automatic
target_include_directories(student_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Custom command to wipe the build directory
if(NOT WIN32)
    add_custom_target(deepclean
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
        COMMENT "Cleaning all build files..."
    )
else()
    add_custom_target(deepclean
        COMMAND del /s /q ${CMAKE_BINARY_DIR}\*
        COMMENT "Cleaning all build files (Windows)..."
    )

endif()
# Automatically copy header.txt to the build directory
configure_file(header.txt ${CMAKE_CURRENT_BINARY_DIR}/header.txt COPYONLY)
