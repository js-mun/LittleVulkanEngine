cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SOURCES "*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/external)

add_executable(a.out ${SOURCES})

target_link_libraries(a.out
    glfw
    vulkan
    dl
    pthread
    X11
    Xxf86vm
    Xrandr
    Xi
)

add_custom_target(run
    COMMAND a.out
    DEPENDS a.out
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

