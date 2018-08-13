include(ExternalProject)

find_package(Threads REQUIRED)

set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/3rdparty)

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE OFF
    LOG_BUILD OFF
)

ExternalProject_Get_Property(googletest source_dir binary_dir)

add_library(gtest INTERFACE)
add_dependencies(gtest googletest)
target_include_directories(gtest INTERFACE ${source_dir}/googletest/include)
target_link_libraries(gtest INTERFACE
    ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${CMAKE_THREAD_LIBS_INIT})

add_library(gtest_main INTERFACE)
target_link_libraries(gtest_main INTERFACE
    ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}
    gtest)
