file(GLOB_RECURSE sources "src/*.cpp")

add_executable(elcad_runtime ${sources})

target_include_directories(elcad_runtime PUBLIC include PRIVATE src)


target_link_libraries(elcad_runtime PRIVATE elcad_ctrl elcad_view elcad_util)