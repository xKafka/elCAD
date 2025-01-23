########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND libzip_COMPONENT_NAMES libzip::zip)
list(REMOVE_DUPLICATES libzip_COMPONENT_NAMES)
if(DEFINED libzip_FIND_DEPENDENCY_NAMES)
  list(APPEND libzip_FIND_DEPENDENCY_NAMES LibLZMA zstd BZip2 ZLIB)
  list(REMOVE_DUPLICATES libzip_FIND_DEPENDENCY_NAMES)
else()
  set(libzip_FIND_DEPENDENCY_NAMES LibLZMA zstd BZip2 ZLIB)
endif()
set(LibLZMA_FIND_MODE "NO_MODULE")
set(zstd_FIND_MODE "NO_MODULE")
set(BZip2_FIND_MODE "NO_MODULE")
set(ZLIB_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(libzip_PACKAGE_FOLDER_DEBUG "C:/Users/filip/.conan2/p/b/libzi5e06a99746eab/p")
set(libzip_BUILD_MODULES_PATHS_DEBUG )


set(libzip_INCLUDE_DIRS_DEBUG "${libzip_PACKAGE_FOLDER_DEBUG}/include")
set(libzip_RES_DIRS_DEBUG )
set(libzip_DEFINITIONS_DEBUG )
set(libzip_SHARED_LINK_FLAGS_DEBUG )
set(libzip_EXE_LINK_FLAGS_DEBUG )
set(libzip_OBJECTS_DEBUG )
set(libzip_COMPILE_DEFINITIONS_DEBUG )
set(libzip_COMPILE_OPTIONS_C_DEBUG )
set(libzip_COMPILE_OPTIONS_CXX_DEBUG )
set(libzip_LIB_DIRS_DEBUG "${libzip_PACKAGE_FOLDER_DEBUG}/lib")
set(libzip_BIN_DIRS_DEBUG )
set(libzip_LIBRARY_TYPE_DEBUG STATIC)
set(libzip_IS_HOST_WINDOWS_DEBUG 1)
set(libzip_LIBS_DEBUG zip)
set(libzip_SYSTEM_LIBS_DEBUG advapi32 bcrypt)
set(libzip_FRAMEWORK_DIRS_DEBUG )
set(libzip_FRAMEWORKS_DEBUG )
set(libzip_BUILD_DIRS_DEBUG )
set(libzip_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(libzip_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libzip_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libzip_COMPILE_OPTIONS_C_DEBUG}>")
set(libzip_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libzip_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libzip_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libzip_EXE_LINK_FLAGS_DEBUG}>")


set(libzip_COMPONENTS_DEBUG libzip::zip)
########### COMPONENT libzip::zip VARIABLES ############################################

set(libzip_libzip_zip_INCLUDE_DIRS_DEBUG "${libzip_PACKAGE_FOLDER_DEBUG}/include")
set(libzip_libzip_zip_LIB_DIRS_DEBUG "${libzip_PACKAGE_FOLDER_DEBUG}/lib")
set(libzip_libzip_zip_BIN_DIRS_DEBUG )
set(libzip_libzip_zip_LIBRARY_TYPE_DEBUG STATIC)
set(libzip_libzip_zip_IS_HOST_WINDOWS_DEBUG 1)
set(libzip_libzip_zip_RES_DIRS_DEBUG )
set(libzip_libzip_zip_DEFINITIONS_DEBUG )
set(libzip_libzip_zip_OBJECTS_DEBUG )
set(libzip_libzip_zip_COMPILE_DEFINITIONS_DEBUG )
set(libzip_libzip_zip_COMPILE_OPTIONS_C_DEBUG "")
set(libzip_libzip_zip_COMPILE_OPTIONS_CXX_DEBUG "")
set(libzip_libzip_zip_LIBS_DEBUG zip)
set(libzip_libzip_zip_SYSTEM_LIBS_DEBUG advapi32 bcrypt)
set(libzip_libzip_zip_FRAMEWORK_DIRS_DEBUG )
set(libzip_libzip_zip_FRAMEWORKS_DEBUG )
set(libzip_libzip_zip_DEPENDENCIES_DEBUG ZLIB::ZLIB BZip2::BZip2 LibLZMA::LibLZMA zstd::libzstd_static)
set(libzip_libzip_zip_SHARED_LINK_FLAGS_DEBUG )
set(libzip_libzip_zip_EXE_LINK_FLAGS_DEBUG )
set(libzip_libzip_zip_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(libzip_libzip_zip_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libzip_libzip_zip_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libzip_libzip_zip_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libzip_libzip_zip_EXE_LINK_FLAGS_DEBUG}>
)
set(libzip_libzip_zip_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libzip_libzip_zip_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libzip_libzip_zip_COMPILE_OPTIONS_C_DEBUG}>")