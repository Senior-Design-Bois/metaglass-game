cmake_minimum_required(VERSION 3.7...3.23)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# set the project name
project(MetaglassGame VERSION 1.0
                  DESCRIPTION "C++ Game Implementation for Metaglass CS Senior Design Class"
                  LANGUAGES CXX)

add_library(section STATIC src/section.cpp include/section.hpp)
target_include_directories(section PUBLIC include)
target_compile_features(section PUBLIC cxx_std_11)

# add the executable
add_executable(Game src/game.cpp)
target_link_libraries(Game PUBLIC section)