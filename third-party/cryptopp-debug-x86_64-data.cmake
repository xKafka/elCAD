########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND cryptopp_COMPONENT_NAMES cryptopp::cryptopp)
list(REMOVE_DUPLICATES cryptopp_COMPONENT_NAMES)
if(DEFINED cryptopp_FIND_DEPENDENCY_NAMES)
  list(APPEND cryptopp_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES cryptopp_FIND_DEPENDENCY_NAMES)
else()
  set(cryptopp_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(cryptopp_PACKAGE_FOLDER_DEBUG "C:/Users/filip/.conan2/p/b/crypt93bce988c5853/p")
set(cryptopp_BUILD_MODULES_PATHS_DEBUG )


set(cryptopp_INCLUDE_DIRS_DEBUG "${cryptopp_PACKAGE_FOLDER_DEBUG}/include")
set(cryptopp_RES_DIRS_DEBUG )
set(cryptopp_DEFINITIONS_DEBUG )
set(cryptopp_SHARED_LINK_FLAGS_DEBUG )
set(cryptopp_EXE_LINK_FLAGS_DEBUG )
set(cryptopp_OBJECTS_DEBUG )
set(cryptopp_COMPILE_DEFINITIONS_DEBUG )
set(cryptopp_COMPILE_OPTIONS_C_DEBUG )
set(cryptopp_COMPILE_OPTIONS_CXX_DEBUG )
set(cryptopp_LIB_DIRS_DEBUG "${cryptopp_PACKAGE_FOLDER_DEBUG}/lib")
set(cryptopp_BIN_DIRS_DEBUG )
set(cryptopp_LIBRARY_TYPE_DEBUG STATIC)
set(cryptopp_IS_HOST_WINDOWS_DEBUG 1)
set(cryptopp_LIBS_DEBUG cryptopp)
set(cryptopp_SYSTEM_LIBS_DEBUG bcrypt ws2_32)
set(cryptopp_FRAMEWORK_DIRS_DEBUG )
set(cryptopp_FRAMEWORKS_DEBUG )
set(cryptopp_BUILD_DIRS_DEBUG )
set(cryptopp_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(cryptopp_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${cryptopp_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${cryptopp_COMPILE_OPTIONS_C_DEBUG}>")
set(cryptopp_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${cryptopp_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${cryptopp_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${cryptopp_EXE_LINK_FLAGS_DEBUG}>")


set(cryptopp_COMPONENTS_DEBUG cryptopp::cryptopp)
########### COMPONENT cryptopp::cryptopp VARIABLES ############################################

set(cryptopp_cryptopp_cryptopp_INCLUDE_DIRS_DEBUG "${cryptopp_PACKAGE_FOLDER_DEBUG}/include")
set(cryptopp_cryptopp_cryptopp_LIB_DIRS_DEBUG "${cryptopp_PACKAGE_FOLDER_DEBUG}/lib")
set(cryptopp_cryptopp_cryptopp_BIN_DIRS_DEBUG )
set(cryptopp_cryptopp_cryptopp_LIBRARY_TYPE_DEBUG STATIC)
set(cryptopp_cryptopp_cryptopp_IS_HOST_WINDOWS_DEBUG 1)
set(cryptopp_cryptopp_cryptopp_RES_DIRS_DEBUG )
set(cryptopp_cryptopp_cryptopp_DEFINITIONS_DEBUG )
set(cryptopp_cryptopp_cryptopp_OBJECTS_DEBUG )
set(cryptopp_cryptopp_cryptopp_COMPILE_DEFINITIONS_DEBUG )
set(cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_C_DEBUG "")
set(cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_CXX_DEBUG "")
set(cryptopp_cryptopp_cryptopp_LIBS_DEBUG cryptopp)
set(cryptopp_cryptopp_cryptopp_SYSTEM_LIBS_DEBUG bcrypt ws2_32)
set(cryptopp_cryptopp_cryptopp_FRAMEWORK_DIRS_DEBUG )
set(cryptopp_cryptopp_cryptopp_FRAMEWORKS_DEBUG )
set(cryptopp_cryptopp_cryptopp_DEPENDENCIES_DEBUG )
set(cryptopp_cryptopp_cryptopp_SHARED_LINK_FLAGS_DEBUG )
set(cryptopp_cryptopp_cryptopp_EXE_LINK_FLAGS_DEBUG )
set(cryptopp_cryptopp_cryptopp_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(cryptopp_cryptopp_cryptopp_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${cryptopp_cryptopp_cryptopp_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${cryptopp_cryptopp_cryptopp_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${cryptopp_cryptopp_cryptopp_EXE_LINK_FLAGS_DEBUG}>
)
set(cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${cryptopp_cryptopp_cryptopp_COMPILE_OPTIONS_C_DEBUG}>")