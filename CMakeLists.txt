cmake_minimum_required(VERSION 3.9)
project(thy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_library(UDT_LIB udt)
find_library(PTHREAD_LIB pthread)

add_executable(Server.out src/server.cpp src/Networking.cpp)
add_executable(Client.out src/client.cpp src/Networking.cpp)

target_link_libraries(Server.out ${UDT_LIB} ${PTHREAD_LIB})
target_link_libraries(Client.out ${UDT_LIB} ${PTHREAD_LIB})
