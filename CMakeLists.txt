cmake_minimum_required(VERSION 3.10)

# set the project name
project("Metaglass Game" VERSION 0.01)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Game game.cpp)