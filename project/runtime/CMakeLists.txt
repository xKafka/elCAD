file(GLOB_RECURSE sources "src/*.cpp")

add_executable(elcad_runtime ${sources})

target_include_directories(elcad_runtime PUBLIC include PRIVATE src)

target_link_libraries(elcad_runtime PRIVATE elcad_ctrl elcad_view elcad_util)

add_custom_command(TARGET elcad_runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${RESOURCE_DIRECTORY} $<TARGET_FILE_DIR:elcad_runtime>/resources
)

install(
  TARGETS elcad_runtime
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)