cmake_minimum_required(VERSION 3.0)

project(netbox LANGUAGES CXX)

option(netbox_BUILD_EXAMPLES "Build library examples" ON)
option(netbox_PCAP_GZIP "Build gzip decoder for pcap files" OFF)
option(netbox_PCAP_LZMA "Build lzma decoder for pcap files" OFF)

# Create library target
add_library(netbox INTERFACE)
target_include_directories(netbox INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(netbox INTERFACE cxx_std_17)

# Support gzip decoding
if (netbox_PCAP_GZIP)
    find_package(ZLIB REQUIRED)
    target_compile_definitions(netbox INTERFACE -Dnetbox_PCAP_GZIP)
    target_include_directories(netbox INTERFACE SYSTEM ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(netbox INTERFACE ${ZLIB_LIBRARIES})
endif()

# Support lzma decoding
if (netbox_PCAP_LZMA)
    find_package(LibLZMA REQUIRED)
    target_compile_definitions(netbox INTERFACE -Dnetbox_PCAP_LZMA)
    target_include_directories(netbox INTERFACE SYSTEM ${LIBLZMA_INCLUDE_DIRS})
    target_link_libraries(netbox INTERFACE ${LIBLZMA_LIBRARIES})
endif()

# Library alias
add_library(ksergey::netbox ALIAS netbox)

# Build examples
if (netbox_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
