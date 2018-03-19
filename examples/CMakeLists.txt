if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native -mtune=native -ffast-math -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -fwhole-program")

add_executable(RxRing rxring.cpp)
target_link_libraries(RxRing ksergey::netbox)
target_compile_options(RxRing PRIVATE -Wall -Wextra)

add_executable(PCAPReader pcap_reader.cpp)
target_link_libraries(PCAPReader ksergey::netbox)
target_compile_options(PCAPReader PRIVATE -Wall -Wextra)

add_executable(PacketSource packet_source.cpp)
target_link_libraries(PacketSource ksergey::netbox)
target_compile_options(PacketSource PRIVATE -Wall -Wextra)

add_executable(Accept accept.cpp)
target_link_libraries(Accept ksergey::netbox)
target_compile_options(Accept PRIVATE -Wall -Wextra)
