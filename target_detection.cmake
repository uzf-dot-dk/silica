set(SILICA_TARGET_ARCH "unknown")

if(CMAKE_SYSTEM_NAME STREQUAL "PICO")
    set(SILICA_TARGET_OS "baremetal")
    if(CMAKE_CXX_COMPILER MATCHES "arm.*none.*eabi")
        set(SILICA_TARGET_ARCH "arm")
    endif()

elseif(WIN32)
    set(SILICA_TARGET_OS "windows")
elseif(APPLE)
    set(SILICA_TARGET_OS "macos")
elseif(UNIX)
    set(SILICA_TARGET_OS "linux")
else()
    set(SILICA_TARGET_OS "none")
endif()



message( INFO " Silica is ${SILICA_TARGET_OS} on ${SILICA_TARGET_ARCH}" )

# x86_64	64-bit x86	AMD64
# AMD64	64-bit x86 (MSVC)	AMD64
# i386, i686	32-bit x86	x86
# armv7l	32-bit ARM	ARM32
# aarch64	64-bit ARM	ARM64
# arm64	64-bit ARM (Windows)	ARM64
# ppc64le	64-bit PowerPC (LE)	POWER
# riscv64	64-bit RISC-V	RISC-V
