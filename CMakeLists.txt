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

# Note I tried to use submodules for this, but it is a pain, so rather rely on individual installs of these libraries
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (sciplot 0.2.0 REQUIRED NO_MODULE)

enable_testing()

# TCLAP is header only, so just needs it's path included
include_directories(BEFORE include tclap/include ${sciplot_DIR}/..)

add_executable(
  polyfit
  src/main.cpp
  src/string-compare.cpp
  src/polynomial.cpp
  
)
target_link_libraries(
  polyfit 
  gtest_main
  Eigen3::Eigen
)
