file(GLOB headers include/*.h)
file(GLOB sources src/*.cpp)

add_library(TwurtleEngine ${sources} ${headers})

target_include_directories(TwurtleEngine PUBLIC include)
