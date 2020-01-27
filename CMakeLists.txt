cmake_minimum_required(VERSION 3.10)

project(OpenGL VERSION 0.1)

# specify c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -Wno-maybe-uninitialized")

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build/bin)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_subdirectory(glad/)
add_subdirectory(glm/)

add_executable(HelloWorld ${PROJECT_SOURCE_DIR}/src/helloworld.cpp)
add_executable(HelloTriangle ${PROJECT_SOURCE_DIR}/src/hellotriangles.cpp)
add_executable(Shaders ${PROJECT_SOURCE_DIR}/src/shaderpractice.cpp)
add_executable(Textures ${PROJECT_SOURCE_DIR}/src/texture.cpp)
add_executable(Transform ${PROJECT_SOURCE_DIR}/src/transformations.cpp)


target_include_directories(HelloWorld PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(HelloTriangle PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Shaders PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Textures PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Transform PRIVATE ${PROJECT_SOURCE_DIR}/include)


target_link_libraries(HelloWorld PRIVATE glad glfw OpenGL::GL glm ${CMAKE_DL_LIBS})
target_link_libraries(HelloTriangle PRIVATE glad glfw OpenGL::GL glm ${CMAKE_DL_LIBS})
target_link_libraries(Shaders PRIVATE glad glfw OpenGL::GL glm ${CMAKE_DL_LIBS})
target_link_libraries(Textures PRIVATE glad glfw OpenGL::GL glm ${CMAKE_DL_LIBS} stdc++fs)
target_link_libraries(Transform PRIVATE glad glfw OpenGL::GL glm ${CMAKE_DL_LIBS} stdc++fs)
