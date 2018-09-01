file(GLOB headers include/*.h)
file(GLOB sources src/*.cpp)

add_library(RandomCatEngine ${sources} ${headers})

target_include_directories(RandomCatEngine PUBLIC include)
