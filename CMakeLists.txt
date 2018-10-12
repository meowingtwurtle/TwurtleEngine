cmake_minimum_required(VERSION 3.0)
project(TwurtleEngine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB sub_projects */CMakeLists.txt)
foreach(sub_project ${sub_projects})
    get_filename_component(directory ${sub_project} DIRECTORY)
    add_subdirectory(${directory})
endforeach()
