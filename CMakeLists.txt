cmake_minimum_required(VERSION 3.14)
project(polyfit)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# All dependent libraries are header only. Installation assumes you've
# used git clone --recurse-submodules when you checked this project out.
# The include path below will find them in the installed location.
# This should probably have an environment variable check for each
# in case the user wants to specify a custom path.
include_directories(BEFORE include tclap/include eigen sciplot)

add_executable(
  polyfit
  src/main.cpp
  src/string-compare.cpp
  src/polynomial.cpp
  src/utils.cpp
)

target_link_libraries(
  polyfit 
  gtest_main
)
