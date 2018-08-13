include(ExternalProject)

find_package(Threads REQUIRED)

set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/3rdparty)

ExternalProject_Add(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.4.1
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_TESTING=OFF
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE OFF
    LOG_BUILD OFF
)

ExternalProject_Get_Property(googlebenchmark source_dir binary_dir)

add_library(benchmark INTERFACE)
target_include_directories(benchmark INTERFACE ${source_dir}/include)
target_link_libraries(benchmark INTERFACE
    ${binary_dir}/src/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(benchmark googlebenchmark)

add_library(benchmark_main INTERFACE)
target_link_libraries(benchmark INTERFACE
    ${binary_dir}/src/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark_main${CMAKE_STATIC_LIBRARY_SUFFIX}
    benchmark)
