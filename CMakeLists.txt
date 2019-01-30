cmake_minimum_required(VERSION 2.8)

set(TARGET_NAME ffs_demo)
project( ${TARGET_NAME} )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#add_compile_options(-Wall -Werror -O2)
set(CMAKE_C_FLAGS " -Wall -Werror -O2")

include_directories( ${PROJECT_SOURCE_DIR}/include)
add_subdirectory( ${PROJECT_SOURCE_DIR}/hal )
add_subdirectory( ${PROJECT_SOURCE_DIR}/libs )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src )
