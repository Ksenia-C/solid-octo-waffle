cmake_minimum_required(VERSION 3.10)
project(SlicerSimple)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    main.cpp
    client.cpp
    router.cpp
    task.cpp
    assignment.cpp
    common.cpp
    AssignmersPolicies/article.cpp
    AssignmersPolicies/petrenko.cpp
    AssignmersPolicies/blocks.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -g)
endif()