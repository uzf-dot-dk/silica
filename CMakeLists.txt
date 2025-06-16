cmake_minimum_required(VERSION 3.16)

project(KjutContainers LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library( kjutContainers
    src/KjutContainers.cpp
    src/KjutCore.cpp
    src/Array.cpp
)

target_include_directories( kjutContainers PUBLIC src/include )

if(NOT ${CMAKE_EXPORT_COMPILE_COMMANDS})

    add_executable(sandbox main.cpp)
    target_link_libraries(sandbox PUBLIC kjutContainers )


    add_subdirectory(tests/googletest/)

    enable_testing()

    macro( create_test FILENAME)
        add_executable( ${FILENAME} tests/${FILENAME}.cpp)
        target_link_libraries(${FILENAME} kjutContainers gtest_main )
        add_test( ${FILENAME} ${FILENAME})
    endmacro()

    create_test( tst_array_fixed_size )
    create_test( tst_array_fixed_size_dynamic_size_interchangability )
endif()

# CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build
#  docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)",target=/mnt/host     --tty --interactive     -v `pwd`:`pwd` -w `pwd` ghcr.io/adobe/hyde:latest bash
#
# hyde  -p compile_commands.json --hyde-update --hyde-yaml-dir=docs/api
# hyde --hyde-validate --hyde-yaml-dir=docs/api  -p build/compile_commands.json `pwd`/src/include/kjut/Array.hpp  --  -I`pwd`/src/include
#
