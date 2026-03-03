# Minitalk

## Introduction

Minitalk is a 42 School project focused on inter-process communication using UNIX signals. The project consists of a client and server program that communicate by sending strings bit by bit using only `SIGUSR1` and `SIGUSR2` signals.

This implementation includes bonus features such as Unicode support and server acknowledgment, integrated directly into the main program.

## Installation

Clone the repository and compile:

```bash
git clone https://github.com/NihalBai/minitalk.git
cd minitalk
make
```

This generates two executables: `server` and `client`.

### Make Commands

```bash
make        # Compile server and client
make clean  # Remove object files
make fclean # Remove object files and executables
make re     # Recompile everything
```

## Usage

### 1. Start the Server

```bash
./server
```

The server will display its PID (Process ID), which you'll need for the client.

### 2. Send Messages from Client

```bash
./client [server_PID] "Your message here"
```

### Example

```bash
# Terminal 1
./server
Server PID: 12345
Waiting for messages...

# Terminal 2
./client 12345 "Hello, 42!"
./client 12345 "Unicode works: 你好 🚀"
```

### Features

- **Signal-based communication**: Uses only `SIGUSR1` and `SIGUSR2`
- **Bit-by-bit transmission**: Each character is sent one bit at a time
- **Unicode support**: Handles extended ASCII and Unicode characters
- **Server acknowledgment**: Server confirms reception back to client
- **Error handling**: Validates PID and handles signal failures
