cmake_minimum_required(VERSION 3.6.0)
project(Intro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -Wpedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Wextra -Wpedantic -O3")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/BasicObjects)
include_directories(${PROJECT_SOURCE_DIR}/build/bin/components/h)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(yaml-cpp-0.8.0)


#set(SOURCE
#    ${PROJECT_SOURCE_DIR}/src/main.cpp
#    ${PROJECT_SOURCE_DIR}/src/Window.cpp
#    ${PROJECT_SOURCE_DIR}/src/Engine.cpp
#    ${PROJECT_SOURCE_DIR}/src/Renderer.cpp
#    ${PROJECT_SOURCE_DIR}/src/BasicGeometry.cpp
#    ${PROJECT_SOURCE_DIR}/src/Settings.cpp
#    ${PROJECT_SOURCE_DIR}/src/Scene.cpp
#    ${PROJECT_SOURCE_DIR}/src/GameObject.cpp
#    ${PROJECT_SOURCE_DIR}/src/ComponentFactory.cpp
#    ${PROJECT_SOURCE_DIR}/src/Component.cpp
#   ${PROJECT_SOURCE_DIR}/src/Transform.cpp
#
#)


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE UTILSOURCES "util/*.cpp")


############
file(GLOB COMPONENT_FILES "build/bin/components/cpp/*.cpp")

set(REG_SOURCE "${CMAKE_BINARY_DIR}/generated_registry.cpp")
set(REG_CONTENT "// Generated file\n")


foreach(FILE ${COMPONENT_FILES})
    get_filename_component(NAME ${FILE} NAME_WE)
    string(APPEND REG_CONTENT "extern void register_${NAME}();\n")
    list(APPEND REG_CALLS "    register_${NAME}();\n")
endforeach()

string(APPEND REG_CONTENT "\nvoid RegisterAllComponents() {\n${REG_CALLS}}\n")

file(WRITE "${REG_SOURCE}.tmp" "${REG_CONTENT}")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${REG_SOURCE}.tmp" "${REG_SOURCE}")
############



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

add_executable(program ${SOURCES} ${COMPONENT_FILES} ${REG_SOURCE})



target_link_libraries(program PUBLIC

    SDL2
    SDL2main
    SDL2_image
    SDL2_ttf
    SDL2_mixer
    yaml-cpp::yaml-cpp

)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_executable(UTIL ${UTILSOURCES})