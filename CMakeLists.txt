cmake_minimum_required(VERSION 3.13)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_HOME_DIRECTORY}/bin
    )

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_HOME_DIRECTORY}/lib
    )


project(transform-benchmark)

add_executable(benchmark-no-parallel benchmark.cpp)
target_include_directories(benchmark-no-parallel PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(benchmark-no-parallel PRIVATE -Wall)

add_custom_command(
    TARGET benchmark-no-parallel POST_BUILD
    COMMAND ./benchmark-no-parallel
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running benchmarks on non parallel version..."
)

find_package(OpenMP)

if(OPENMP_FOUND)
    add_executable(benchmark-parallel benchmark.cpp)
    target_include_directories(benchmark-parallel PRIVATE ${CMAKE_SOURCE_DIR})
    target_compile_options(benchmark-parallel PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_libraries(benchmark-parallel PRIVATE ${OpenMP_CXX_LIBRARIES})
    add_custom_command(
        TARGET benchmark-parallel POST_BUILD
        COMMAND ./benchmark-parallel
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running benchmarks on parallel version..."
    )

else()
    message(WARNING "OpenMP not found. Cannot run benchmarks without it")
endif()

