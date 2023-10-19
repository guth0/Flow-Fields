cmake_minimum_required(VERSION 3.12)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Specify the include and library directories for PortAudio
link_directories(/opt/homebrew/Cellar/portaudio/19.7.0/lib)
include_directories(/opt/homebrew/Cellar/portaudio/19.7.0/include)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(main main.cpp renderer.h system.h particle.h noise.h sounds.h)

# Include SFML headers
target_include_directories(main PRIVATE ${SFML_INCLUDE_DIR})

target_link_libraries(main sfml-graphics)

# Link your target with the PortAudio library
target_link_libraries(main portaudio)

