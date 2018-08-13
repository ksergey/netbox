include(GoogleTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(tests_srcs test_ipv4.cpp)
add_executable(unit_tests ${tests_srcs})
target_link_libraries(unit_tests netbox gtest gtest_main)
add_test(UnitTests unit_tests)
