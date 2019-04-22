include(../engine.cmake)
def_engine_lib(Textures)

link_glew()
target_link_libraries(${RC_TARGET} stb RandomCat::Engine::LowLevel RandomCat::All GSL stdc++fs)
