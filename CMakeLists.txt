cmake_minimum_required(VERSION 3.2)
project(framework)

# Request a C++ 14 compliant compiler
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Show compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Produce the executable in a seperate directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add project source code in the src directory
add_subdirectory(src)
