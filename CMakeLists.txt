cmake_minimum_required(VERSION 3.27)
project(Nettverksprogrammering)

set(CMAKE_CXX_STANDARD 17)

add_executable(Nettverksprogrammering main.cpp
        oving_1/is_prime.cpp
        oving_1/prime_calculations.cpp
        oving_2/Workers.cpp
        oving_2/Task.cpp)
