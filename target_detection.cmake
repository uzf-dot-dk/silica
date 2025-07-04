if(WIN32)
    set(KJUT_TARGET_OS "windows")
elseif(UNIX AND NOT APPLE)
    set(KJUT_TARGET_OS "linux")
elseif(APPLE)
    set(KJUT_TARGET_OS "macos")
else()
    set(KJUT_TARGET_OS "none")
endif()


# x86_64	64-bit x86	AMD64
# AMD64	64-bit x86 (MSVC)	AMD64
# i386, i686	32-bit x86	x86
# armv7l	32-bit ARM	ARM32
# aarch64	64-bit ARM	ARM64
# arm64	64-bit ARM (Windows)	ARM64
# ppc64le	64-bit PowerPC (LE)	POWER
# riscv64	64-bit RISC-V	RISC-V
