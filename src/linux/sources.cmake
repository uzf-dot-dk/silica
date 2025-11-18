set( SILICA_OS_ARCH_PREFIX linux)

set( HERE src/${SILICA_OS_ARCH_PREFIX} )
set( silica_sources
    ${silica_sources}
    ${HERE}/${SILICA_OS_ARCH_PREFIX}_application.cpp
    ${HERE}/${SILICA_OS_ARCH_PREFIX}_event_logging.cpp
    ${HERE}/${SILICA_OS_ARCH_PREFIX}_Mutex.cpp
)
