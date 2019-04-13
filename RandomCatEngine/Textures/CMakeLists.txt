project(RandomCatEngineTextures)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE impl_headers include*.inl)
file(GLOB_RECURSE sources src/*.cpp)

add_library(__RC_Engine_Textures ${sources} ${headers} ${impl_headers})

target_include_directories(__RC_Engine_Textures PUBLIC include)
set_target_properties(__RC_Engine_Textures PROPERTIES LINKER_LANGUAGE CXX)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(__RC_Engine_Textures ${GLEW_LIBRARY} ${GLEW_LIB} stb RandomCat::Engine::LowLevel RandomCat::All GSL stdc++fs)
target_compile_options(__RC_Engine_Textures PRIVATE -Wall -Wextra)

add_library(RandomCat::Engine::Textures ALIAS __RC_Engine_Textures)

cmake_policy(SET CMP0079 NEW)
target_link_libraries(__RC_Engine_All INTERFACE __RC_Engine_Textures)
