project(RandomCatEngineUtilities)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE impl_headers include*.inl)
file(GLOB_RECURSE sources src/*.cpp)

add_library(__RC_Engine_Utilities ${sources} ${headers} ${impl_headers})
set_target_properties(__RC_Engine_Input PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(__RC_Engine_Utilities PUBLIC include)
set_target_properties(__RC_Engine_Utilities PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(__RC_Engine_Utilities RandomCat::Engine::LowLevel RandomCat::Engine::RenderObjects glm RandomCat::All GSL)
target_compile_options(__RC_Engine_Utilities PRIVATE -Wall -Wextra)

add_library(RandomCat::Engine::Utilities ALIAS __RC_Engine_Utilities)

cmake_policy(SET CMP0079 NEW)
target_link_libraries(__RC_Engine_All INTERFACE __RC_Engine_Utilities)
