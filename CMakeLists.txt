cmake_minimum_required(VERSION 3.26)
project(Projeto_DA)

set(CMAKE_CXX_STANDARD 17)

add_executable(Projeto_DA src/main.cpp
        src/Graph.h
        src/MutablePriorityQueue.h
        src/Reservoir.h
        src/Reservoir.cpp
        src/Station.h
        src/Station.cpp
        src/City.h
        src/City.cpp
        src/Data.h
        src/Data.cpp
        src/Menu.h
        src/Menu.cpp
)
