# PrankCrypt

**PrankCrypt** is a simple C++ tool that demonstrates RSA encryption and network communication. It encrypts a message, sends it over a network, and decrypts it on the receiving end. This project is a fun way to learn about cryptography and socket programming.

## How It Works

1. The program uses **RSA** to encrypt a message with a public key.
2. It sends the encrypted message to a specified IP and port.
3. The receiver decrypts the message using a private key.

## How to Run

1. **Compile the code**:
   - For MSVC: `cl prankcrypt.cpp ws2_32.lib`
   - For MinGW: `g++ prankcrypt.cpp -o prankcrypt.exe -lws2_32`

2. **Run the program**:
   - Open two terminals.
   - In the first terminal, run the receiver: `prankcrypt.exe`
   - In the second terminal, run the sender: `prankcrypt.exe`

3. Watch the encrypted message get sent and decrypted!

## Requirements

- **Windows** (uses Winsock2)
- **C++ compiler** (MSVC or MinGW)
