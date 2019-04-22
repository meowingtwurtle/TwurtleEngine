include(../engine.cmake)
def_engine_lib(Utilities)

target_link_libraries(${RC_TARGET} RandomCat::Engine::LowLevel RandomCat::Engine::RenderObjects glm RandomCat::All GSL)
