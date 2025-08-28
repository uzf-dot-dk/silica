cmake_minimum_required(VERSION 3.16)

project(KjutContainers LANGUAGES CXX)

include(target_detection.cmake)


option(BUILD_DOCUMENTATION "builds the project for documentation" OFF)
option(BUILD_TESTS         "builds the project for documentation" ON )

if ( ${BUILD_DOCUMENTATION} )
    set(  CMAKE_EXPORT_COMPILE_COMMANDS ON )
    set( BUILD_TESTS OFF )
    message( STATUS "Builds for documentation ")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set( kjut_container_sources
    src/KjutApplication.cpp
    src/KjutCoarseTimer.cpp
    src/KjutContainers.cpp
    src/KjutCore.cpp
    src/KjutEventGenerator.cpp
    src/KjutLogEntry.cpp
    src/KjutLoggingSystem.cpp
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

if( ${BUILD_TESTS} )
    message( STATUS "Builds tests")

    target_compile_definitions(kjutContainers PUBLIC KJUT_LOGENTRY_MESSAGE_MAX_LENGTH=15)
    target_compile_definitions(kjutContainers PUBLIC KJUT_LOGENTRY_FILENAME_MAX_LENGTH=15)

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
    create_test( tst_array_different_types )
    create_test( tst_set )
    create_test( tst_ringbuffer )
    create_test( tst_map )
    create_test( tst_signals_and_slots )
    create_test( tst_application )
    create_test( tst_logentry )

endif()
# CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build
#  docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)",target=/mnt/host     --tty --interactive     -v `pwd`:`pwd` -w `pwd` ghcr.io/adobe/hyde:latest bash
#
# hyde  -p compile_commands.json --hyde-update --hyde-yaml-dir=docs/api
# hyde --hyde-validate --hyde-yaml-dir=docs/api  -p build/compile_commands.json `pwd`/src/include/kjut/Array.hpp  --  -I`pwd`/src/include
#
