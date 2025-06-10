cmake_minimum_required(VERSION 4.0.2)

project(STLClone CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only STL-style library
add_library(stl_containers INTERFACE)
target_include_directories(stl_containers INTERFACE include)

# Add GoogleTest 
add_subdirectory(external/googletest)

enable_testing()

# Test binary
add_executable(stl_tests
    src/containers/vector_tests.cpp
    src/memory/allocator_tests.cpp
)

target_link_libraries(stl_tests
  PRIVATE
    stl_containers
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME STLContainerTests COMMAND stl_tests)
