cmake_minimum_required(VERSION 3.0)

project(NetBoxLANGUAGES CXX)

option(NetBox_BUILD_EXAMPLES "Build library examples" ON)
option(NetBox_PCAP_GZIP "Build gzip decoder for pcap files" OFF)
option(NetBox_PCAP_LZMA "Build lzma decoder for pcap files" OFF)

# Create library target
add_library(NetBox INTERFACE)
target_include_directories(NetBox INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(NetBox INTERFACE cxx_std_17)

# Support gzip decoding
if (NetBox_PCAP_GZIP)
    find_package(ZLIB REQUIRED)
    target_compile_definitions(NetBox INTERFACE -DNetBox_PCAP_GZIP)
    target_include_directories(NetBox INTERFACE SYSTEM ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(NetBox INTERFACE ${ZLIB_LIBRARIES})
endif()

# Support lzma decoding
if (NetBox_PCAP_LZMA)
    find_package(LibLZMA REQUIRED)
    target_compile_definitions(NetBox INTERFACE -DNetBox_PCAP_LZMA)
    target_include_directories(NetBox INTERFACE SYSTEM ${LIBLZMA_INCLUDE_DIRS})
    target_link_libraries(NetBox INTERFACE ${LIBLZMA_LIBRARIES})
endif()

# Library alias
add_library(ksergey::NetBox ALIAS NetBox)

# Build examples
if (NetBox_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
