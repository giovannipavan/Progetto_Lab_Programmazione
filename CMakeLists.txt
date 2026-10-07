cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ProgettoDomotica)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-O2)

# Indica le directory contenenti i file header
include_directories(include)

add_executable(ProgettoDomotica
    src/Main.cpp
    src/Elettrodomestico.cpp
    src/Manuale.cpp
    src/CP.cpp
    src/Casa_1.cpp
    src/Casa_2.cpp
)
