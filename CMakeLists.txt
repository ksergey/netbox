cmake_minimum_required(VERSION 3.0)

project(netbox LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(netbox_BUILD_EXAMPLES "Build library examples" ON)
option(netbox_PCAP_GZIP "Build gzip decoder for pcap files" OFF)
option(netbox_PCAP_LZMA "Build lzma decoder for pcap files" OFF)
option(netbox_BUILD_TESTS "Build tests" ON)

# Library dir
set(netbox_dir ${CMAKE_CURRENT_SOURCE_DIR}/netbox)

# Create library target
add_library(netbox INTERFACE)
target_include_directories(netbox INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(netbox INTERFACE cxx_std_17)

target_sources(netbox
    INTERFACE
        ${netbox_dir}/buffer.h
        ${netbox_dir}/compiler.h
        ${netbox_dir}/ConsumingBuffer.h
        ${netbox_dir}/debug.h
        ${netbox_dir}/details/byte_order.h
        ${netbox_dir}/details/concepts.h
        ${netbox_dir}/details/ipv4/Address.h
        ${netbox_dir}/details/ipv4/Endpoint.h
        ${netbox_dir}/details/ipv6/Address.h
        ${netbox_dir}/details/ipv6/Endpoint.h
        ${netbox_dir}/details/socket_options.h
        ${netbox_dir}/ErrorCode.h
        ${netbox_dir}/exception.h
        ${netbox_dir}/IPv4.h
        ${netbox_dir}/IPv6.h
        ${netbox_dir}/pcap/Packet.h
        ${netbox_dir}/PcapPacketSource.h
        ${netbox_dir}/pcap/pcap.h
        ${netbox_dir}/pcap/Reader.h
        ${netbox_dir}/pdu/EthernetII.h
        ${netbox_dir}/pdu/IPv4.h
        ${netbox_dir}/pdu/UDP.h
        ${netbox_dir}/Protocol.h
        ${netbox_dir}/resolve.h
        ${netbox_dir}/result.h
        ${netbox_dir}/Socket.h
        ${netbox_dir}/socket_ops.h
        ${netbox_dir}/socket_options.h
        ${netbox_dir}/StaticBuffer.h
        ${netbox_dir}/utils/FileReader.h
        ${netbox_dir}/utils/GZipDecompressStream.h
        ${netbox_dir}/utils/LZMADecompressStream.h
        ${netbox_dir}/utils/string.h
)

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

# Build tests
if (netbox_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
