file(GLOB_RECURSE sources "src/*.cpp")

find_package(Vulkan REQUIRED)

find_package(spdlog CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

add_library(elcad_framework STATIC ${sources})

target_include_directories(elcad_framework PUBLIC include PRIVATE src)

target_include_directories(elcad_framework PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(elcad_framework PUBLIC ${Vulkan_LIBRARIES})

target_link_libraries(elcad_framework PUBLIC elcad_util)

target_link_libraries(elcad_framework PUBLIC 
spdlog::spdlog
glfw
)