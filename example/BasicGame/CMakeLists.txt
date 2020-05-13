project(BasicGame)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.hpp)

add_executable(BasicGame ${sources} ${headers})

target_link_libraries(BasicGame RandomCat::Engine::All GL glm stdc++fs RandomCat::Engine::Textures)
target_compile_options(
        BasicGame
        PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wnull-dereference
        -Wformat=2
        -Weffc++
        -Wnarrowing
)

set(UseOptimizer FALSE)
set(UseLTO TRUE)
set(UseSanitizers FALSE)

if (UseOptimizer)
    target_compile_options(BasicGame PRIVATE -O3)

    if (UseLTO)
        target_compile_options(BasicGame PRIVATE -flto)
        target_link_options(BasicGame PRIVATE -flto)
    endif ()
else ()
    target_compile_options(BasicGame PRIVATE -O0)
endif ()

if (UseSanitizers)
    set(SanitizerChecks address,undefined,integer)
    set(SanitizerArgs -fsanitize=${SanitizerChecks} -fno-sanitize-recover=${SanitizerChecks})

    target_compile_options(BasicGame PRIVATE ${SanitizerArgs})
    target_link_options(BasicGame PRIVATE ${SanitizerArgs})
endif ()
