#pragma once

#include <winsock2.h>

SOCKET Client(const char* addresses, const char* port);
std::string getResp(SOCKET clientsocket);
std::string request(SOCKET clientsocket, std::string text);