# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(cryptopp_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(cryptopp_FRAMEWORKS_FOUND_DEBUG "${cryptopp_FRAMEWORKS_DEBUG}" "${cryptopp_FRAMEWORK_DIRS_DEBUG}")

set(cryptopp_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET cryptopp_DEPS_TARGET)
    add_library(cryptopp_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET cryptopp_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${cryptopp_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${cryptopp_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### cryptopp_DEPS_TARGET to all of them
conan_package_library_targets("${cryptopp_LIBS_DEBUG}"    # libraries
                              "${cryptopp_LIB_DIRS_DEBUG}" # package_libdir
                              "${cryptopp_BIN_DIRS_DEBUG}" # package_bindir
                              "${cryptopp_LIBRARY_TYPE_DEBUG}"
                              "${cryptopp_IS_HOST_WINDOWS_DEBUG}"
                              cryptopp_DEPS_TARGET
                              cryptopp_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "cryptopp"    # package_name
                              "${cryptopp_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${cryptopp_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT cryptopp::cryptopp #############

        set(cryptopp_cryptopp_cryptopp_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(cryptopp_cryptopp_cryptopp_FRAMEWORKS_FOUND_DEBUG "${cryptopp_cryptopp_cryptopp_FRAMEWORKS_DEBUG}" "${cryptopp_cryptopp_cryptopp_FRAMEWORK_DIRS_DEBUG}")

        set(cryptopp_cryptopp_cryptopp_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET cryptopp_cryptopp_cryptopp_DEPS_TARGET)
            add_library(cryptopp_cryptopp_cryptopp_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET cryptopp_cryptopp_cryptopp_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'cryptopp_cryptopp_cryptopp_DEPS_TARGET' to all of them
        conan_package_library_targets("${cryptopp_cryptopp_cryptopp_LIBS_DEBUG}"
                              "${cryptopp_cryptopp_cryptopp_LIB_DIRS_DEBUG}"
                              "${cryptopp_cryptopp_cryptopp_BIN_DIRS_DEBUG}" # package_bindir
                              "${cryptopp_cryptopp_cryptopp_LIBRARY_TYPE_DEBUG}"
                              "${cryptopp_cryptopp_cryptopp_IS_HOST_WINDOWS_DEBUG}"
                              cryptopp_cryptopp_cryptopp_DEPS_TARGET
                              cryptopp_cryptopp_cryptopp_LIBRARIES_TARGETS
                              "_DEBUG"
                              "cryptopp_cryptopp_cryptopp"
                              "${cryptopp_cryptopp_cryptopp_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET cryptopp::cryptopp
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_LIBRARIES_TARGETS}>
                     )

        if("${cryptopp_cryptopp_cryptopp_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET cryptopp::cryptopp
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         cryptopp_cryptopp_cryptopp_DEPS_TARGET)
        endif()

        set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_LIB_DIRS_DEBUG}>)
        set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET cryptopp::cryptopp APPEND PROPERTY INTERFACE_LINK_LIBRARIES cryptopp::cryptopp)

########## For the modules (FindXXX)
set(cryptopp_LIBRARIES_DEBUG cryptopp::cryptopp)
