cmake_minimum_required(VERSION 3.10)
project(MapMiniGame)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/client.cpp
    src/server.cpp
    src/socketsUtil.cpp
)

# Executables
add_executable(server src/server.cpp src/socketsUtil.cpp)
add_executable(client src/client.cpp src/socketsUtil.cpp)

# Link libraries if needed (e.g., pthread)
target_link_libraries(server pthread)
target_link_libraries(client pthread)
