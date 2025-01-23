########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(cxxopts_COMPONENT_NAMES "")
if(DEFINED cxxopts_FIND_DEPENDENCY_NAMES)
  list(APPEND cxxopts_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES cxxopts_FIND_DEPENDENCY_NAMES)
else()
  set(cxxopts_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(cxxopts_PACKAGE_FOLDER_DEBUG "C:/Users/filip/.conan2/p/cxxop782c19b5a4d3b/p")
set(cxxopts_BUILD_MODULES_PATHS_DEBUG )


set(cxxopts_INCLUDE_DIRS_DEBUG "${cxxopts_PACKAGE_FOLDER_DEBUG}/include")
set(cxxopts_RES_DIRS_DEBUG )
set(cxxopts_DEFINITIONS_DEBUG )
set(cxxopts_SHARED_LINK_FLAGS_DEBUG )
set(cxxopts_EXE_LINK_FLAGS_DEBUG )
set(cxxopts_OBJECTS_DEBUG )
set(cxxopts_COMPILE_DEFINITIONS_DEBUG )
set(cxxopts_COMPILE_OPTIONS_C_DEBUG )
set(cxxopts_COMPILE_OPTIONS_CXX_DEBUG )
set(cxxopts_LIB_DIRS_DEBUG )
set(cxxopts_BIN_DIRS_DEBUG )
set(cxxopts_LIBRARY_TYPE_DEBUG UNKNOWN)
set(cxxopts_IS_HOST_WINDOWS_DEBUG 1)
set(cxxopts_LIBS_DEBUG )
set(cxxopts_SYSTEM_LIBS_DEBUG )
set(cxxopts_FRAMEWORK_DIRS_DEBUG )
set(cxxopts_FRAMEWORKS_DEBUG )
set(cxxopts_BUILD_DIRS_DEBUG )
set(cxxopts_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(cxxopts_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${cxxopts_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${cxxopts_COMPILE_OPTIONS_C_DEBUG}>")
set(cxxopts_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${cxxopts_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${cxxopts_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${cxxopts_EXE_LINK_FLAGS_DEBUG}>")


set(cxxopts_COMPONENTS_DEBUG )