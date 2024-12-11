set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE sources "src/*.cpp")

add_library(river_framework STATIC ${sources})

target_include_directories(river_framework PUBLIC include PRIVATE src)

target_link_libraries(river_framework PRIVATE lakeware_shared_static)
