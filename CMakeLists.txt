cmake_minimum_required( VERSION 3.19 )
project( pymulti )


include_directories(
    ${PROJECT_SOURCE_DIR}/src/${PY}
)

file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/src/${PY}/*.c"
)

add_executable( ${PROJECT_NAME} ${all_SRCS} )

include(CMakePrintHelpers)

set(Python_USE_STATIC_LIBS "ON")
find_package(Python 3.${PY} REQUIRED COMPONENTS Development.Embed)

include_directories(${Python_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Python::Python)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

