cmake_minimum_required(VERSION 3.16)

project(KjutContainers LANGUAGES CXX)

include(target_detection.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set( kjut_container_sources
    src/KjutContainers.cpp
    src/KjutCore.cpp
    src/KjutMutex.cpp
)

if( ${KJUT_TARGET_OS} STREQUAL "windows")
    include(src/windows_x86/sources.cmake)
endif()

if( ${KJUT_TARGET_OS} STREQUAL "linux")
    include(src/linux/sources.cmake)
endif()

add_library( kjutContainers
    ${kjut_container_sources}
)

target_include_directories( kjutContainers PUBLIC src/include )
target_compile_definitions(kjutContainers PUBLIC KJUT_TARGET_OS=\"${KJUT_TARGET_OS}\")

if( ${KJUT_TARGET_OS} STREQUAL "windows")
    target_compile_definitions(kjutContainers PUBLIC KJUT_OS_WINDOWS=1)
endif()

if( ${KJUT_TARGET_OS} STREQUAL "linux")
    target_compile_definitions(kjutContainers PUBLIC KJUT_OS_LINUX=1)
endif()



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
create_test( tst_array_dynamic_size )
create_test( tst_array_fixed_size_dynamic_size_interchangability )
create_test( tst_set )
create_test( tst_ringbuffer )

# CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build
#  docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)",target=/mnt/host     --tty --interactive     -v `pwd`:`pwd` -w `pwd` ghcr.io/adobe/hyde:latest bash
#
# hyde  -p compile_commands.json --hyde-update --hyde-yaml-dir=docs/api
# hyde --hyde-validate --hyde-yaml-dir=docs/api  -p build/compile_commands.json `pwd`/src/include/kjut/Array.hpp  --  -I`pwd`/src/include
#
