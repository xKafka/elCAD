########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(portable-file-dialogs_FIND_QUIETLY)
    set(portable-file-dialogs_MESSAGE_MODE VERBOSE)
else()
    set(portable-file-dialogs_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/portable-file-dialogsTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${portable-file-dialogs_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(portable-file-dialogs_VERSION_STRING "0.1.0")
set(portable-file-dialogs_INCLUDE_DIRS ${portable-file-dialogs_INCLUDE_DIRS_DEBUG} )
set(portable-file-dialogs_INCLUDE_DIR ${portable-file-dialogs_INCLUDE_DIRS_DEBUG} )
set(portable-file-dialogs_LIBRARIES ${portable-file-dialogs_LIBRARIES_DEBUG} )
set(portable-file-dialogs_DEFINITIONS ${portable-file-dialogs_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${portable-file-dialogs_BUILD_MODULES_PATHS_DEBUG} )
    message(${portable-file-dialogs_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


