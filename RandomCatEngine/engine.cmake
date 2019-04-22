macro(def_engine_lib name)
    project(RandomCatEngine${name})

    file(GLOB_RECURSE headers include/*.hpp)
    file(GLOB_RECURSE impl_headers include*.inl)
    file(GLOB_RECURSE sources src/*.cpp)

    set(RC_TARGET __RC_Engine_${name})

    add_library(${RC_TARGET} ${sources} ${headers} ${impl_headers})

    set_target_properties(${RC_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(${RC_TARGET} PUBLIC include)
    target_compile_options(${RC_TARGET} PRIVATE -Wall -Wextra)

    add_library(RandomCat::Engine::${name} ALIAS ${RC_TARGET})

    cmake_policy(SET CMP0079 NEW)
    target_link_libraries(__RC_Engine_All INTERFACE ${RC_TARGET})
endmacro()

macro(link_sdl)
    find_package(SDL2 REQUIRED)
    target_link_libraries(${RC_TARGET} ${SDL2_LIBRARIES})
endmacro()

macro(link_glew)
    find_package(GLEW REQUIRED)
    target_link_libraries(${RC_TARGET} ${GLEW_LIBRARY} ${GLEW_LIB})
endmacro()