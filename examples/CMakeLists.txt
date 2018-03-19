if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

add_executable(RxRing rxring.cpp)
target_link_libraries(RxRing ksergey::NetBox)
target_compile_options(RxRing PRIVATE -Wall -Wextra)

add_executable(PCAPReader pcap_reader.cpp)
target_link_libraries(PCAPReader ksergey::NetBox)
target_compile_options(PCAPReader PRIVATE -Wall -Wextra)

add_executable(Accept accept.cpp)
target_link_libraries(Accept ksergey::NetBox)
target_compile_options(Accept PRIVATE -Wall -Wextra)
