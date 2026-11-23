cmake_minimum_required(VERSION 3.10)
project(shop_lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8 /W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

file(GLOB SHOP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SHOP_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_library(shop_core STATIC ${SHOP_SOURCES})
target_include_directories(shop_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(shop_lab3 src/main.cpp)
target_link_libraries(shop_lab3 shop_core)

add_executable(run_tests tests/run_tests.cpp)
target_link_libraries(run_tests shop_core)

enable_testing()
add_test(NAME shop_tests COMMAND run_tests)
