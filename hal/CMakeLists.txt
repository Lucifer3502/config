cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME hal)

set(COMMON_INCLUDE "${PROJECT_SOURCE_DIR}/include")
file(GLOB SOURCE_FILES "*.c" "*.cpp" "*.cc")
file(GLOB PRIVATE_HEADERS "*.h" "*.hpp")
file(GLOB PUBLIC_HEADERS "inc/*.h" "include/*.hpp")
file(GLOB COMMON_HEADERS "${COMMON_INCLUDE}/*.h" "${COMMON_INCLUDE}/*.hpp")

add_definitions(-DHAL_CONF_FILE_PATHNAME="${PROJECT_BINARY_DIR}/bin/flash.bin")
add_library( ${TARGET_NAME} STATIC  ${SOURCE_FILES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS} ${COMMON_HEADERS})
target_include_directories(${TARGET_NAME} PUBLIC "inc")
target_link_libraries( ${TARGET_NAME} )

