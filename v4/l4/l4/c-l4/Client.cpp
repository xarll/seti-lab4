#define WIN32_LEAN_AND_MEAN

#include <iostream>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "Client.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

std::string getResp(SOCKET clientsocket);


// фция для обрезки строки
char* slice(char* s, int from, int to)
{
    int j = 0;
    for (int i = from; i <= to; ++i)
        s[j++] = s[i];
    s[j] = 0;
    return s;
}

SOCKET Client(const char* addresses, const char* port)
{

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    int iResult;

    // Инициализация Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // адрес сервера и порт
    iResult = getaddrinfo(addresses, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Пытаться подключиться к адресу, пока не удастся
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Создать SOCKET для подключения к серверу
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Подключиться к серверу
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        // std::cout << "Успешно подключился к " << addresses << ":" << port << std::endl;
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    return ConnectSocket;
}


std::string request(SOCKET clientsocket, std::string text)
{

    char* cstr = &text[0];
    int iResult;

    iResult = send(clientsocket, cstr, (int)strlen(cstr), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(clientsocket);
        WSACleanup();
        return "";
    }


    // отключение соединения, так как данные больше не будут передаваться
    iResult = shutdown(clientsocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientsocket);
        WSACleanup();
    }

    std::string respMessage = getResp(clientsocket);

    return respMessage;
}


std::string getResp(SOCKET clientsocket)
{
    int iResult;
    std::string message;

    // Размер принимаемого буфера
    const int DEFAULT_BUFLEN = 512;

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN];

    // Принимать до тех пор, пока одноранговый узел не закроет соединение
    do {

        iResult = recv(clientsocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            message += slice(recvbuf, 0, iResult - 1);
        else if (iResult == -1)
            printf("recv failed with error: %d\n", WSAGetLastError());
            
    } while (iResult > 0);
    
    return message;
}