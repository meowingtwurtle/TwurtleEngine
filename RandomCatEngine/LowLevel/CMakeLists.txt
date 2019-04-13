project(RandomCatEngineLowLevel)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE impl_headers include*.inl)
file(GLOB_RECURSE sources src/*.cpp)

add_library(__RC_Engine_LowLevel ${sources} ${headers} ${impl_headers})
set_target_properties(__RC_Engine_LowLevel PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(__RC_Engine_LowLevel PUBLIC include)

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(__RC_Engine_LowLevel ${GLEW_LIBRARY} ${GLEW_LIB} ${SDL2_LIBRARIES} RandomCat::All GSL glm)
target_compile_options(__RC_Engine_LowLevel PRIVATE -Wall -Wextra)

add_library(RandomCat::Engine::LowLevel ALIAS __RC_Engine_LowLevel)

cmake_policy(SET CMP0079 NEW)
target_link_libraries(__RC_Engine_All INTERFACE __RC_Engine_LowLevel)
