cmake_minimum_required(VERSION 3.16)

project(KjutContainers LANGUAGES CXX)

add_subdirectory(tests/googletest/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library( kjutContainers
    src/KjutContainers.cpp
    src/KjutCore.cpp
)

target_include_directories( kjutContainers PUBLIC src/include )

add_executable(sandbox main.cpp)
target_link_libraries(sandbox PUBLIC kjutContainers )


enable_testing()

macro( create_test FILENAME)
    add_executable( ${FILENAME} tests/${FILENAME}.cpp)
    target_link_libraries(${FILENAME} kjutContainers gtest_main )
    add_test( ${FILENAME} ${FILENAME})
endmacro()

create_test( tst_array_fixed_size )
