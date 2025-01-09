cmake_minimum_required(VERSION 3.10)
project(MapMiniGame)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files (common sources)
set(COMMON_SOURCES
    src/GameMap.cpp
    src/GameState.cpp
    util/socketsUtil.cpp
)

# Server executable
add_executable(server src/server.cpp ${COMMON_SOURCES})

# Client executable
add_executable(client src/client.cpp ${COMMON_SOURCES})

# Test executable (gameStateTest)
add_executable(gameStateTest tests/gameStateTest.cpp ${COMMON_SOURCES})

# Link libraries if needed (e.g., pthread)
target_link_libraries(server pthread)
target_link_libraries(client pthread)
