cmake_minimum_required(VERSION 3.3)
project(lua_bind)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp lua_bind.cpp lua_bind.h ./3rd/rapidjson/include/rapidjson)

add_executable(lua_bind ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(lua_bind liblua.a)