#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

vector<long long> rsa_encrypt(const string& message, long long e, long long n) {
    vector<long long> encrypted;
    for (char c : message) {
        long long m = static_cast<long long>(c);
        long long ciphered = mod_pow(m, e, n);
        encrypted.push_back(ciphered);
    }
    return encrypted;
}

string rsa_decrypt(const vector<long long>& encrypted, long long d, long long n) {
    string decrypted;
    for (long long c : encrypted) {
        long long m = mod_pow(c, d, n);
        decrypted += static_cast<char>(m);
    }
    return decrypted;
}

void send_message(const string& ip, int port, const vector<long long>& message) {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Connection failed." << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    string msg_str;
    for (long long c : message) {
        msg_str += to_string(c) + " ";
    }
    send(sock, msg_str.c_str(), msg_str.size(), 0);

    closesocket(sock);
    WSACleanup();
}

vector<long long> receive_message(int port) {
    WSADATA wsaData;
    SOCKET sock, client_sock;
    sockaddr_in server, client;
    int client_len = sizeof(client);
    char buffer[1024] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return {};
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return {};
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Bind failed." << endl;
        closesocket(sock);
        WSACleanup();
        return {};
    }

    listen(sock, 1);
    cout << "Waiting for incoming connections..." << endl;

    if ((client_sock = accept(sock, (sockaddr*)&client, &client_len)) < 0) {
        cerr << "Accept failed." << endl;
        closesocket(sock);
        WSACleanup();
        return {};
    }

    recv(client_sock, buffer, 1024, 0);
    string msg_str(buffer);
    vector<long long> encrypted;
    size_t pos = 0;
    while ((pos = msg_str.find(' ')) != string::npos) {
        string token = msg_str.substr(0, pos);
        if (!token.empty()) {
            encrypted.push_back(stoll(token));
        }
        msg_str.erase(0, pos + 1);
    }

    closesocket(client_sock);
    closesocket(sock);
    WSACleanup();

    return encrypted;
}

int main() {
    long long e = 19;
    long long d = 2753;
    long long n = 3233;

    string message = "Hello!";
    cout << "Original message: " << message << endl;

    vector<long long> encrypted = rsa_encrypt(message, e, n);
    cout << "Encrypted message: ";
    for (long long c : encrypted) {
        cout << c << " ";
    }
    cout << endl;

    send_message("127.0.0.1", 8080, encrypted);

    vector<long long> received = receive_message(8080);

    string decrypted = rsa_decrypt(received, d, n);
    cout << "Decrypted message: " << decrypted << endl;

    return 0;
}
