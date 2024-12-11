file(GLOB_RECURSE sources "src/*.cpp")

add_library(elcad_ctrl STATIC ${sources} ${headers})

target_include_directories(elcad_ctrl PUBLIC include PRIVATE src)

target_link_libraries(elcad_ctrl PUBLIC elcad_view elcad_util elcad_framework)