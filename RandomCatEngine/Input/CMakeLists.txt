include(../engine.cmake)
def_engine_lib(Input)

link_sdl()
target_link_libraries(${RC_TARGET} RandomCat::Engine::LowLevel)
