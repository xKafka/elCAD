# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libzip_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(libzip_FRAMEWORKS_FOUND_DEBUG "${libzip_FRAMEWORKS_DEBUG}" "${libzip_FRAMEWORK_DIRS_DEBUG}")

set(libzip_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libzip_DEPS_TARGET)
    add_library(libzip_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libzip_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${libzip_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${libzip_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:ZLIB::ZLIB;BZip2::BZip2;LibLZMA::LibLZMA;zstd::libzstd_static>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libzip_DEPS_TARGET to all of them
conan_package_library_targets("${libzip_LIBS_DEBUG}"    # libraries
                              "${libzip_LIB_DIRS_DEBUG}" # package_libdir
                              "${libzip_BIN_DIRS_DEBUG}" # package_bindir
                              "${libzip_LIBRARY_TYPE_DEBUG}"
                              "${libzip_IS_HOST_WINDOWS_DEBUG}"
                              libzip_DEPS_TARGET
                              libzip_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "libzip"    # package_name
                              "${libzip_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libzip_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT libzip::zip #############

        set(libzip_libzip_zip_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(libzip_libzip_zip_FRAMEWORKS_FOUND_DEBUG "${libzip_libzip_zip_FRAMEWORKS_DEBUG}" "${libzip_libzip_zip_FRAMEWORK_DIRS_DEBUG}")

        set(libzip_libzip_zip_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET libzip_libzip_zip_DEPS_TARGET)
            add_library(libzip_libzip_zip_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET libzip_libzip_zip_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'libzip_libzip_zip_DEPS_TARGET' to all of them
        conan_package_library_targets("${libzip_libzip_zip_LIBS_DEBUG}"
                              "${libzip_libzip_zip_LIB_DIRS_DEBUG}"
                              "${libzip_libzip_zip_BIN_DIRS_DEBUG}" # package_bindir
                              "${libzip_libzip_zip_LIBRARY_TYPE_DEBUG}"
                              "${libzip_libzip_zip_IS_HOST_WINDOWS_DEBUG}"
                              libzip_libzip_zip_DEPS_TARGET
                              libzip_libzip_zip_LIBRARIES_TARGETS
                              "_DEBUG"
                              "libzip_libzip_zip"
                              "${libzip_libzip_zip_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET libzip::zip
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_LIBRARIES_TARGETS}>
                     )

        if("${libzip_libzip_zip_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET libzip::zip
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         libzip_libzip_zip_DEPS_TARGET)
        endif()

        set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_LIB_DIRS_DEBUG}>)
        set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${libzip_libzip_zip_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET libzip::zip APPEND PROPERTY INTERFACE_LINK_LIBRARIES libzip::zip)

########## For the modules (FindXXX)
set(libzip_LIBRARIES_DEBUG libzip::zip)
