project(stb)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE sources src/*.cpp)

add_library(stb ${sources} ${headers})
target_include_directories(stb PUBLIC include)
