cmake_minimum_required(VERSION 3.6)
project(dafuweng)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

add_executable(dafuweng main.cpp dafuweng.h purchase.cpp query.cpp move.cpp map.cpp judge.cpp mine.cpp gift_house.cpp)