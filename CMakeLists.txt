cmake_minimum_required(VERSION 3.14)
project(polyfit)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)

# Setup testing
#enable_testing()
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})

#add_executable( polytest src/string-compare.cpp)
#target_link_libraries( polytest gtest_main)


# All dependent libraries are header only. Installation assumes you've
# used git clone --recurse-submodules when you checked this project out.
# The include path below will find them in the installed location.
# This should probably have an environment variable check for each
# in case the user wants to specify a custom path.
include_directories(BEFORE include 
	dependencies/tclap/include 
	dependencies/eigen 
	dependencies/sciplot 
	dependencies/rapidcsv/src
  dependencies/acutest/include)

add_executable(
  polyfit
  src/main.cpp
  src/polynomial.cpp
  src/utils.cpp
)
