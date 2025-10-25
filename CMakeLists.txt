cmake_minimum_required(VERSION 3.16)

project(rr)

set(ALL_SRC 
  rr/main.cpp
  rr/messages.cpp
  rr/arg_traits.cpp
)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
add_executable(rr ${ALL_SRC})
