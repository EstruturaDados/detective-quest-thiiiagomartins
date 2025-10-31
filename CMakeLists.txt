cmake_minimum_required(VERSION 4.0)
project(detective-quest-thiiiagomartins C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(detective-quest-thiiiagomartins
    src/detective_quest.c
    src/sala.c
    src/pistas.c
    src/suspeito.c
        include/navegacao.h
        src/navegacao.c
)
