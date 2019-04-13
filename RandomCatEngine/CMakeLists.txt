cmake_minimum_required(VERSION 3.0)
project(RandomCatEngineL)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

add_library(__RC_Engine_All INTERFACE)
add_library(RandomCat::Engine::All ALIAS __RC_Engine_All)

file(GLOB sub_projects */CMakeLists.txt)
foreach(sub_project ${sub_projects})
    get_filename_component(directory ${sub_project} DIRECTORY)
    add_subdirectory(${directory})
endforeach()
