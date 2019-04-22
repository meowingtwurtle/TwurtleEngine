include(../engine.cmake)
def_engine_lib(RenderObjects)

target_link_libraries(${RC_TARGET} PUBLIC RandomCat::Engine::LowLevel RandomCat::Engine::Textures)
