project(engine)

file(GLOB_RECURSE headers include/*.hpp)
file(GLOB_RECURSE impl_headers include*.inl)
file(GLOB_RECURSE sources src/*.cpp)

add_library(RandomCatEngine ${sources} ${headers} ${impl_headers})

target_include_directories(RandomCatEngine PUBLIC include)

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(RandomCatEngine ${GLEW_LIBRARY} ${GLEW_LIB} ${SDL2_LIBRARIES} glm stb RandomCat::All GSL)
target_compile_options(RandomCatEngine PRIVATE -Wall -Wextra)
