cmake_minimum_required(VERSION 3.10)

project(DiningPhilosophers CXX)

add_subdirectory(DiningPhilosophers)

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -header-filter=.;
  -checks=*;
  -warnings-as-errors=*;
)

include(CPack)