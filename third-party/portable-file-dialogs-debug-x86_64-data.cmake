########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(portable-file-dialogs_COMPONENT_NAMES "")
if(DEFINED portable-file-dialogs_FIND_DEPENDENCY_NAMES)
  list(APPEND portable-file-dialogs_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES portable-file-dialogs_FIND_DEPENDENCY_NAMES)
else()
  set(portable-file-dialogs_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(portable-file-dialogs_PACKAGE_FOLDER_DEBUG "C:/Users/filip/.conan2/p/portaadfef820f5c0f/p")
set(portable-file-dialogs_BUILD_MODULES_PATHS_DEBUG )


set(portable-file-dialogs_INCLUDE_DIRS_DEBUG "${portable-file-dialogs_PACKAGE_FOLDER_DEBUG}/include")
set(portable-file-dialogs_RES_DIRS_DEBUG )
set(portable-file-dialogs_DEFINITIONS_DEBUG )
set(portable-file-dialogs_SHARED_LINK_FLAGS_DEBUG )
set(portable-file-dialogs_EXE_LINK_FLAGS_DEBUG )
set(portable-file-dialogs_OBJECTS_DEBUG )
set(portable-file-dialogs_COMPILE_DEFINITIONS_DEBUG )
set(portable-file-dialogs_COMPILE_OPTIONS_C_DEBUG )
set(portable-file-dialogs_COMPILE_OPTIONS_CXX_DEBUG )
set(portable-file-dialogs_LIB_DIRS_DEBUG )
set(portable-file-dialogs_BIN_DIRS_DEBUG )
set(portable-file-dialogs_LIBRARY_TYPE_DEBUG UNKNOWN)
set(portable-file-dialogs_IS_HOST_WINDOWS_DEBUG 1)
set(portable-file-dialogs_LIBS_DEBUG )
set(portable-file-dialogs_SYSTEM_LIBS_DEBUG )
set(portable-file-dialogs_FRAMEWORK_DIRS_DEBUG )
set(portable-file-dialogs_FRAMEWORKS_DEBUG )
set(portable-file-dialogs_BUILD_DIRS_DEBUG )
set(portable-file-dialogs_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(portable-file-dialogs_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${portable-file-dialogs_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${portable-file-dialogs_COMPILE_OPTIONS_C_DEBUG}>")
set(portable-file-dialogs_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${portable-file-dialogs_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${portable-file-dialogs_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${portable-file-dialogs_EXE_LINK_FLAGS_DEBUG}>")


set(portable-file-dialogs_COMPONENTS_DEBUG )