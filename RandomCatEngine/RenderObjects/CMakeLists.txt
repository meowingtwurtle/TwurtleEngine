project(RandomCatEngineRenderObjects)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE impl_headers include*.inl)
file(GLOB_RECURSE sources src/*.cpp)

add_library(__RC_Engine_RenderObjects ${sources} ${headers} ${impl_headers})

target_include_directories(__RC_Engine_RenderObjects PUBLIC include)
set_target_properties(__RC_Engine_RenderObjects PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(__RC_Engine_RenderObjects PUBLIC RandomCat::Engine::LowLevel RandomCat::Engine::Textures)
target_compile_options(__RC_Engine_RenderObjects PRIVATE -Wall -Wextra)

add_library(RandomCat::Engine::RenderObjects ALIAS __RC_Engine_RenderObjects)

cmake_policy(SET CMP0079 NEW)
target_link_libraries(__RC_Engine_All INTERFACE __RC_Engine_RenderObjects)
