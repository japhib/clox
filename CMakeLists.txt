cmake_minimum_required(VERSION 3.17)
project(clox C)

set(CMAKE_C_STANDARD 99)

# turn on all warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -g -O0)
endif()

add_executable(clox
    main.c
    common.h
    compiler.h compiler.c
    chunk.h chunk.c
    debug.h debug.c
    memory.h memory.c
    scanner.h scanner.c
    value.h value.c
    vm.h vm.c
)
