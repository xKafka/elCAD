########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(libzip_FIND_QUIETLY)
    set(libzip_MESSAGE_MODE VERBOSE)
else()
    set(libzip_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/libzipTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${libzip_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(libzip_VERSION_STRING "1.11.1")
set(libzip_INCLUDE_DIRS ${libzip_INCLUDE_DIRS_DEBUG} )
set(libzip_INCLUDE_DIR ${libzip_INCLUDE_DIRS_DEBUG} )
set(libzip_LIBRARIES ${libzip_LIBRARIES_DEBUG} )
set(libzip_DEFINITIONS ${libzip_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${libzip_BUILD_MODULES_PATHS_DEBUG} )
    message(${libzip_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


