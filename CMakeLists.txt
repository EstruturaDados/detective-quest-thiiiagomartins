cmake_minimum_required(VERSION 4.0)
project(detective-quest-thiiiagomartins C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(detective-quest-thiiiagomartins
    utils.c
    detective_quest.c
    detective_quest.h
)
