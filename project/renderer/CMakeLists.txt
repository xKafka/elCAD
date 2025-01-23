file(GLOB_RECURSE sources "src/*.cpp")

find_package(Vulkan REQUIRED)

find_package(spdlog CONFIG REQUIRED)

add_library(elcad_renderer STATIC ${sources})

target_include_directories(elcad_renderer PUBLIC include PRIVATE src)

target_include_directories(elcad_renderer PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(elcad_renderer PUBLIC ${Vulkan_LIBRARIES})

target_link_libraries(elcad_renderer PUBLIC elcad_util elcad_model)

target_link_libraries(elcad_renderer PUBLIC 
spdlog::spdlog

)