cmake_minimum_required( VERSION 3.19 )
project( pymulti )

set(THREADS $ENV{THREADS})

if(NOT THREADS)
set(THREADS 4)
endif()

configure_file(src/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h @ONLY)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/${PY}
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/src/${PY}/*.c"
    "${PROJECT_SOURCE_DIR}/*.h"
)

add_executable( ${PROJECT_NAME} ${all_SRCS} )

include(CMakePrintHelpers)

set(Python_USE_STATIC_LIBS "ON")
find_package(Python 3.${PY} REQUIRED COMPONENTS Development.Embed)

include_directories(${Python_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Python::Python)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

