cmake_minimum_required(VERSION 3.16)
project(TourneySim VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Werror -Wextra -Wpedantic -Wno-error=vla)
# Optimizations
add_compile_options(-march=native -Ofast -fomit-frame-pointer -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse -DUSE_AVX2 -mavx2)
#Not default optimizations. Enable for slight boost on supported systems. Aprox 64ms. From 2828 to 2764 on average.
#add_compile_options( -DUSE_AVX512 -mavx512f)

add_executable(TourneySim "")

#[[
target_link_libraries(Quanticade
    PRIVATE
        nnue
        m
) ]]

set(TOURNEYSIM_SOURCES
    "Source/main.c"
    "Source/misc.c"
    "Source/formulas.c"
    "Source/tourney.c"
)

set(TOURNEYSIM_HEADERS
    "Source/defs.h"
)

set(TOURNEYSIM_TEXTFILES
    "Source/players.txt"
)

target_sources(TourneySim
    PRIVATE
        ${TOURNEYSIM_SOURCES}
    PUBLIC
        ${TOURNEYSIM_HEADERS}
        ${TOURNEYSIM_TEXTFILES}
)
