# Conan automatically generated toolchain file
# DO NOT EDIT MANUALLY, it will be overwritten

# Avoid including toolchain file several times (bad if appending to variables like
#   CMAKE_CXX_FLAGS. See https://github.com/android/ndk/issues/323
include_guard()
message(STATUS "Using Conan toolchain: ${CMAKE_CURRENT_LIST_FILE}")
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeToolchain' generator only works with CMake >= 3.15")
endif()

########## 'user_toolchain' block #############
# Include one or more CMake user toolchain from tools.cmake.cmaketoolchain:user_toolchain



########## 'generic_system' block #############
# Definition of system, platform and toolset





########## 'compilers' block #############



########## 'libcxx' block #############
# Definition of libcxx from 'compiler.libcxx' setting, defining the
# right CXX_FLAGS for that libcxx



########## 'vs_runtime' block #############
# Definition of VS runtime CMAKE_MSVC_RUNTIME_LIBRARY, from settings build_type,
# compiler.runtime, compiler.runtime_type

cmake_policy(GET CMP0091 POLICY_CMP0091)
if(NOT "${POLICY_CMP0091}" STREQUAL NEW)
    message(FATAL_ERROR "The CMake policy CMP0091 must be NEW, but is '${POLICY_CMP0091}'")
endif()
message(STATUS "Conan toolchain: Setting CMAKE_MSVC_RUNTIME_LIBRARY=$<$<CONFIG:Debug>:MultiThreadedDebugDLL>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDebugDLL>")


########## 'cppstd' block #############
# Define the C++ and C standards from 'compiler.cppstd' and 'compiler.cstd'

function(conan_modify_std_watch variable access value current_list_file stack)
    set(conan_watched_std_variable 23)
    if (${variable} STREQUAL "CMAKE_C_STANDARD")
        set(conan_watched_std_variable )
    endif()
    if (${access} STREQUAL "MODIFIED_ACCESS" AND NOT ${value} STREQUAL ${conan_watched_std_variable})
        message(STATUS "Warning: Standard ${variable} value defined in conan_toolchain.cmake to ${conan_watched_std_variable} has been modified to ${value} by ${current_list_file}")
    endif()
    unset(conan_watched_std_variable)
endfunction()

message(STATUS "Conan toolchain: C++ Standard 23 with extensions OFF")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
variable_watch(CMAKE_CXX_STANDARD conan_modify_std_watch)


########## 'extra_flags' block #############
# Include extra C++, C and linker flags from configuration tools.build:<type>flags
# and from CMakeToolchain.extra_<type>_flags

# Conan conf flags start: 
# Conan conf flags end


########## 'cmake_flags_init' block #############
# Define CMAKE_<XXX>_FLAGS from CONAN_<XXX>_FLAGS

foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${config} config)
    if(DEFINED CONAN_CXX_FLAGS_${config})
      string(APPEND CMAKE_CXX_FLAGS_${config}_INIT " ${CONAN_CXX_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_C_FLAGS_${config})
      string(APPEND CMAKE_C_FLAGS_${config}_INIT " ${CONAN_C_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_SHARED_LINKER_FLAGS_${config})
      string(APPEND CMAKE_SHARED_LINKER_FLAGS_${config}_INIT " ${CONAN_SHARED_LINKER_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_EXE_LINKER_FLAGS_${config})
      string(APPEND CMAKE_EXE_LINKER_FLAGS_${config}_INIT " ${CONAN_EXE_LINKER_FLAGS_${config}}")
    endif()
endforeach()

if(DEFINED CONAN_CXX_FLAGS)
  string(APPEND CMAKE_CXX_FLAGS_INIT " ${CONAN_CXX_FLAGS}")
endif()
if(DEFINED CONAN_C_FLAGS)
  string(APPEND CMAKE_C_FLAGS_INIT " ${CONAN_C_FLAGS}")
endif()
if(DEFINED CONAN_SHARED_LINKER_FLAGS)
  string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " ${CONAN_SHARED_LINKER_FLAGS}")
endif()
if(DEFINED CONAN_EXE_LINKER_FLAGS)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " ${CONAN_EXE_LINKER_FLAGS}")
endif()


########## 'extra_variables' block #############
# Definition of extra CMake variables from tools.cmake.cmaketoolchain:extra_variables



########## 'try_compile' block #############
# Blocks after this one will not be added when running CMake try/checks

get_property( _CMAKE_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE )
if(_CMAKE_IN_TRY_COMPILE)
    message(STATUS "Running toolchain IN_TRY_COMPILE")
    return()
endif()


########## 'find_paths' block #############
# Define paths to find packages, programs, libraries, etc.

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Definition of CMAKE_MODULE_PATH
# the generators folder (where conan generates files, like this toolchain)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# Definition of CMAKE_PREFIX_PATH, CMAKE_XXXXX_PATH
# The Conan local "generators" folder, where this toolchain is saved.
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR} )
list(PREPEND CMAKE_PROGRAM_PATH "C:/Users/filip/.conan2/p/cmake1927748c2604b/p/bin")
list(PREPEND CMAKE_LIBRARY_PATH "C:/Users/filip/.conan2/p/b/spdlobf6da8d388b1c/p/lib" "C:/Users/filip/.conan2/p/b/fmt1f193baa082f5/p/lib" "C:/Users/filip/.conan2/p/b/crypt93bce988c5853/p/lib" "C:/Users/filip/.conan2/p/b/libzi5e06a99746eab/p/lib" "C:/Users/filip/.conan2/p/b/xz_ut04e33c9fca027/p/lib" "C:/Users/filip/.conan2/p/b/zstd7060b223e766a/p/lib" "C:/Users/filip/.conan2/p/b/assim67c3b5d9a4c1f/p/lib" "C:/Users/filip/.conan2/p/b/miniz711a3f7969f92/p/lib" "C:/Users/filip/.conan2/p/b/bzip23e4a47f3a024b/p/lib" "C:/Users/filip/.conan2/p/b/pugix97fc5d1fd27cc/p/lib" "lib" "C:/Users/filip/.conan2/p/b/zlib0f9097edb0c6c/p/lib" "C:/Users/filip/.conan2/p/b/kuba-14c029297b09d/p/lib" "C:/Users/filip/.conan2/p/b/poly2415d463025f2c/p/lib" "lib" "C:/Users/filip/.conan2/p/b/draco291509c4d1860/p/lib" "C:/Users/filip/.conan2/p/b/clippb1650b1362cf8/p/lib" "C:/Users/filip/.conan2/p/b/openda4787283c30a4/p/lib" "C:/Users/filip/.conan2/p/b/gtest660d755becb63/p/lib")
list(PREPEND CMAKE_INCLUDE_PATH "C:/Users/filip/.conan2/p/b/spdlobf6da8d388b1c/p/include" "C:/Users/filip/.conan2/p/b/fmt1f193baa082f5/p/include" "C:/Users/filip/.conan2/p/glmdf97922a12435/p/include" "C:/Users/filip/.conan2/p/nlohm0567ffc90cfc1/p/include" "C:/Users/filip/.conan2/p/cxxop782c19b5a4d3b/p/include" "C:/Users/filip/.conan2/p/b/crypt93bce988c5853/p/include" "C:/Users/filip/.conan2/p/b/libzi5e06a99746eab/p/include" "C:/Users/filip/.conan2/p/b/xz_ut04e33c9fca027/p/include" "C:/Users/filip/.conan2/p/b/zstd7060b223e766a/p/include" "C:/Users/filip/.conan2/p/b/assim67c3b5d9a4c1f/p/include" "C:/Users/filip/.conan2/p/b/miniz711a3f7969f92/p/include" "C:/Users/filip/.conan2/p/b/miniz711a3f7969f92/p/include/minizip" "C:/Users/filip/.conan2/p/b/bzip23e4a47f3a024b/p/include" "C:/Users/filip/.conan2/p/b/pugix97fc5d1fd27cc/p/include" "include" "C:/Users/filip/.conan2/p/b/zlib0f9097edb0c6c/p/include" "C:/Users/filip/.conan2/p/b/kuba-14c029297b09d/p/include" "C:/Users/filip/.conan2/p/b/poly2415d463025f2c/p/include" "include" "C:/Users/filip/.conan2/p/b/draco291509c4d1860/p/include" "C:/Users/filip/.conan2/p/b/clippb1650b1362cf8/p/include" "C:/Users/filip/.conan2/p/stb6342cecb318f5/p/include" "C:/Users/filip/.conan2/p/b/openda4787283c30a4/p/include" "C:/Users/filip/.conan2/p/b/gtest660d755becb63/p/include" "C:/Users/filip/.conan2/p/portaadfef820f5c0f/p/include")
set(CONAN_RUNTIME_LIB_DIRS "C:/Users/filip/.conan2/p/b/spdlobf6da8d388b1c/p/bin" "C:/Users/filip/.conan2/p/b/fmt1f193baa082f5/p/bin" "C:/Users/filip/.conan2/p/b/crypt93bce988c5853/p/bin" "C:/Users/filip/.conan2/p/b/libzi5e06a99746eab/p/bin" "C:/Users/filip/.conan2/p/b/xz_ut04e33c9fca027/p/bin" "C:/Users/filip/.conan2/p/b/zstd7060b223e766a/p/bin" "C:/Users/filip/.conan2/p/b/assim67c3b5d9a4c1f/p/bin" "C:/Users/filip/.conan2/p/b/miniz711a3f7969f92/p/bin" "C:/Users/filip/.conan2/p/b/bzip23e4a47f3a024b/p/bin" "C:/Users/filip/.conan2/p/b/pugix97fc5d1fd27cc/p/bin" "bin" "C:/Users/filip/.conan2/p/b/zlib0f9097edb0c6c/p/bin" "C:/Users/filip/.conan2/p/b/kuba-14c029297b09d/p/bin" "C:/Users/filip/.conan2/p/b/poly2415d463025f2c/p/bin" "bin" "C:/Users/filip/.conan2/p/b/draco291509c4d1860/p/bin" "C:/Users/filip/.conan2/p/b/clippb1650b1362cf8/p/bin" "C:/Users/filip/.conan2/p/b/openda4787283c30a4/p/bin" "C:/Users/filip/.conan2/p/b/gtest660d755becb63/p/bin" )



########## 'pkg_config' block #############
# Define pkg-config from 'tools.gnu:pkg_config' executable and paths

if (DEFINED ENV{PKG_CONFIG_PATH})
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR};$ENV{PKG_CONFIG_PATH}")
else()
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR};")
endif()


########## 'rpath' block #############
# Defining CMAKE_SKIP_RPATH



########## 'output_dirs' block #############
# Definition of CMAKE_INSTALL_XXX folders

set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_SBINDIR "bin")
set(CMAKE_INSTALL_LIBEXECDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_INCLUDEDIR "include")
set(CMAKE_INSTALL_OLDINCLUDEDIR "include")


########## 'variables' block #############
# Definition of CMake variables from CMakeToolchain.variables values

# Variables
# Variables  per configuration



########## 'preprocessor' block #############
# Preprocessor definitions from CMakeToolchain.preprocessor_definitions values

# Preprocessor definitions per configuration



if(CMAKE_POLICY_DEFAULT_CMP0091)  # Avoid unused and not-initialized warnings
endif()
