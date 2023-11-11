cmake_minimum_required(VERSION 3.12)
project(FlowFields)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(main main.cpp renderer.h system.h particle.h spawner.h simulation.h)

# Include SFML headers
target_include_directories(main PRIVATE ${SFML_INCLUDE_DIR})

target_link_libraries(main sfml-graphics)


