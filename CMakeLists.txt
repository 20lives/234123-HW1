cmake_minimum_required(VERSION 3.16)
project(hw1_234123)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw1_234123 smash.cpp Commands.cpp signals.cpp BuiltInCommand.cpp Utilities.cpp ExternalCommand.cpp SmallShell.cpp JobsList.cpp)