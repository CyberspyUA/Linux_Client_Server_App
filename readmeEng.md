# TCP Server-Client Program

This program implements a simple TCP server and client in C using socket programming in Linux.

## Features
- Server listens for incoming connections and responds based on client messages.
- Clients can connect to the server and send messages.
- If a client sends 'hello', the server responds 'world'. Otherwise, the server echoes back the received message.
- Both server and client can maintain a TCP connection and exchange messages until the user sends a 'disconnect' command.

## Usage
1. Compile the server and client programs using the provided Makefile:
    ```bash
    make
    ```

2. Run the server:
    ```bash
    ./server
    ```

3. In another terminal window, run the client:
    ```bash
    ./client
    ```

4. Enter messages in the client terminal and observe the server responses.
5. To disconnect from the server, enter 'disconnect' in the client terminal.

## Cleanup
To clean up the compiled binaries, run:
```bash
make clean

