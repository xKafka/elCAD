# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(portable-file-dialogs_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(portable-file-dialogs_FRAMEWORKS_FOUND_DEBUG "${portable-file-dialogs_FRAMEWORKS_DEBUG}" "${portable-file-dialogs_FRAMEWORK_DIRS_DEBUG}")

set(portable-file-dialogs_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET portable-file-dialogs_DEPS_TARGET)
    add_library(portable-file-dialogs_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET portable-file-dialogs_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${portable-file-dialogs_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${portable-file-dialogs_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### portable-file-dialogs_DEPS_TARGET to all of them
conan_package_library_targets("${portable-file-dialogs_LIBS_DEBUG}"    # libraries
                              "${portable-file-dialogs_LIB_DIRS_DEBUG}" # package_libdir
                              "${portable-file-dialogs_BIN_DIRS_DEBUG}" # package_bindir
                              "${portable-file-dialogs_LIBRARY_TYPE_DEBUG}"
                              "${portable-file-dialogs_IS_HOST_WINDOWS_DEBUG}"
                              portable-file-dialogs_DEPS_TARGET
                              portable-file-dialogs_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "portable-file-dialogs"    # package_name
                              "${portable-file-dialogs_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${portable-file-dialogs_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${portable-file-dialogs_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${portable-file-dialogs_LIBRARIES_TARGETS}>
                 )

    if("${portable-file-dialogs_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET portable-file-dialogs::portable-file-dialogs
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     portable-file-dialogs_DEPS_TARGET)
    endif()

    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${portable-file-dialogs_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${portable-file-dialogs_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${portable-file-dialogs_LIB_DIRS_DEBUG}>)
    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${portable-file-dialogs_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET portable-file-dialogs::portable-file-dialogs
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${portable-file-dialogs_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(portable-file-dialogs_LIBRARIES_DEBUG portable-file-dialogs::portable-file-dialogs)
