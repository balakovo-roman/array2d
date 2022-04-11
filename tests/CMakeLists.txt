cmake_minimum_required(VERSION 3.16)

project(tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

enable_testing()

add_executable(tests main.cpp)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/array ${GTEST_INCLUDE_DIRS})

target_link_libraries(tests PRIVATE array gtest gtest_main Threads::Threads)

gtest_discover_tests(tests)
