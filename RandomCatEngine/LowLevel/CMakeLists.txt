include(../engine.cmake)
def_engine_lib(LowLevel)

link_sdl()
link_glew()
target_link_libraries(${RC_TARGET} RandomCat::All GSL glm)
