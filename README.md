# Mini Game Sockets

This project is a simple game that uses sockets for communication between a server and multiple clients. As of now there are no game features yet but players can move around in a multiplayer grid.

## Features

- Multiplayer support
- Real-time communication

## Requirements

- C++17
- CMake
- POSIX-compliant system (e.g., Linux)

## How to Build

1. Clone the repository:
    ```bash
    git clone https://github.com/gabnash05/FlatMultiplayer.git
    cd FlatMultiplayer
    ```

2. Create a build directory and navigate into it:
    ```bash
    mkdir build && cd build
    ```

3. Run CMake to configure the project:
    ```bash
    cmake ..
    ```

4. Build the project:
    ```bash
    make
    ```

## How to Run

1. Start the server:
    ```bash
    ./server
    ```

2. Start the client(s):
    ```bash
    ./client
    ```