cmake_minimum_required(VERSION 3.21...3.27)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(elcad VERSION 2.2.0 DESCRIPTION "" LANGUAGES CXX C)

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resource)

add_subdirectory(configured-files)
add_subdirectory(project)
