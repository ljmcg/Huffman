cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)
add_library(huffman huffman.cpp)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE huffman)

 enable_testing()

add_test(NAME tests
         COMMAND tests)


add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld PRIVATE fmt::fmt)
