cmake_minimum_required(VERSION 3.17)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw1 smash.cpp Commands.cpp Commands.h signals.cpp signals.h)