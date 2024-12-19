file(GLOB_RECURSE sources "src/*.cpp")

find_package(fmt CONFIG REQUIRED)

find_package(glm REQUIRED)

add_library(elcad_util STATIC ${sources})

target_include_directories(elcad_util PUBLIC include PRIVATE src)

target_link_libraries(elcad_util PUBLIC 
fmt::fmt
glm::glm
)