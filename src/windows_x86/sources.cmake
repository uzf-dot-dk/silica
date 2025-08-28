set( KJUT_OS_ARCH_PREFIX windows_x86)

set( HERE src/${KJUT_OS_ARCH_PREFIX} )
set( kjut_container_sources
    ${kjut_container_sources}
    ${HERE}/${KJUT_OS_ARCH_PREFIX}_Mutex.cpp
    ${HERE}/${KJUT_OS_ARCH_PREFIX}_event_logging.cpp
    ${HERE}/${KJUT_OS_ARCH_PREFIX}_application.cpp
	 
)
