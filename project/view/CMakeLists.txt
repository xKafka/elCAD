file(GLOB_RECURSE sources "src/*.cpp")

find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS} ${Vulkan_LIBRARIES}")
else()
    message(FATAL_ERROR "Vulkan not found")
endif()

find_package(glfw3 CONFIG REQUIRED)

add_library(elcad_view STATIC ${sources})

target_include_directories(elcad_view PUBLIC include PRIVATE src)

target_include_directories(elcad_view PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(elcad_view PUBLIC 
glfw
)

target_link_libraries(elcad_view PUBLIC ${Vulkan_LIBRARIES})

target_link_libraries(elcad_view PUBLIC elcad_ctrl elcad_util elcad_framework)
