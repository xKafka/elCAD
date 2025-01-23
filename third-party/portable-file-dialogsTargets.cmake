# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/portable-file-dialogs-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${portable-file-dialogs_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${portable-file-dialogs_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET portable-file-dialogs::portable-file-dialogs)
    add_library(portable-file-dialogs::portable-file-dialogs INTERFACE IMPORTED)
    message(${portable-file-dialogs_MESSAGE_MODE} "Conan: Target declared 'portable-file-dialogs::portable-file-dialogs'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/portable-file-dialogs-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()