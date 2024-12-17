set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE sources "src/*.cpp")

add_library(elcad_model STATIC ${sources})

target_include_directories(elcad_model PUBLIC include PRIVATE src)

target_link_libraries(elcad_model PUBLIC elcad_util)