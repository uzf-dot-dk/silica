cmake_minimum_required(VERSION 3.16)

project(Silica LANGUAGES CXX)

include(target_detection.cmake)


option(BUILD_DOCUMENTATION "builds the project for documentation" OFF)
option(BUILD_TESTS         "builds the project for documentation" ON )

if ( ${BUILD_DOCUMENTATION} )
    set(  CMAKE_EXPORT_COMPILE_COMMANDS ON )
    set( BUILD_TESTS OFF )
    message( STATUS "Builds for documentation ")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set( silica_sources
    src/SilicaApplication.cpp
    src/SilicaByteArray.cpp
    src/SilicaByteBuffer.cpp
    src/SilicaCoarseTimer.cpp
    src/SilicaContainers.cpp
    src/SilicaCore.cpp
    src/SilicaEventGenerator.cpp
    src/SilicaIODevice.cpp
    src/SilicaLogEntry.cpp
    src/SilicaLoggingSystem.cpp
    src/SilicaMutex.cpp
    src/SilicaUnitsOfTime.cpp
)

if( ${SILICA_TARGET_OS} STREQUAL "windows")
    include(src/windows_x86/sources.cmake)
endif()

if( ${SILICA_TARGET_OS} STREQUAL "linux")
    message ( INFO " Oncludes for linux")
    include(src/linux/sources.cmake)
endif()

add_library( silica
    ${silica_sources}
)

target_include_directories( silica PUBLIC src/include )
target_compile_definitions( silica PUBLIC SILICA_TARGET_OS=\"${SILICA_TARGET_OS}\")
if(MSVC)
    target_compile_options( silica PRIVATE /Od /RTC1)
endif()

if( ${SILICA_TARGET_OS} STREQUAL "windows")
    target_compile_definitions( silica PUBLIC SILICA_OS_WINDOWS=1)
endif()

if( ${SILICA_TARGET_OS} STREQUAL "linux")
    target_compile_definitions( silica PUBLIC SILICA_OS_LINUX=1)
endif()

if ( ${SILICA_BUILD_SANDBOX} )
    add_executable(sandbox main.cpp)
    target_link_libraries(sandbox PUBLIC silica )
endif()

if( ${SILICA_BUILD_TESTS} )
    message( STATUS "Builds tests")

    target_compile_definitions( silica PUBLIC SILICA_LOGENTRY_MESSAGE_MAX_LENGTH=15)
    target_compile_definitions( silica PUBLIC SILICA_LOGENTRY_FILENAME_MAX_LENGTH=15)

    add_subdirectory(tests/googletest/)

    enable_testing()

    macro( create_test FILENAME)
        add_executable( ${FILENAME} tests/${FILENAME}.cpp)
        target_link_libraries(${FILENAME} silica gtest_main )
        add_test( ${FILENAME} ${FILENAME})
        if(MSVC)
            target_compile_options(${FILENAME} PRIVATE /Od /RTC1)
        endif()
    endmacro()

    create_test( tst_application )
    create_test( tst_array_fixed_size )
    create_test( tst_array_dynamic_size )
    create_test( tst_array_fixed_size_dynamic_size_interchangability )
    create_test( tst_array_different_types )
    create_test( tst_byte_array )
    create_test( tst_byte_buffer )
    create_test( tst_coarse_timer )
    create_test( tst_logentry )
    create_test( tst_map )
    create_test( tst_ringbuffer )
    create_test( tst_set )
    create_test( tst_signals_and_slots )
    create_test( tst_text_based_api )

endif()
	
