#include <iostream>

#include "Client.h"

int main()
{
    setlocale(LC_ALL, "rus");

    std::string phrase = "nologin bread";

    const char* addresses = "10.185.239.66";
    const char* port = "27015";

    std::string result = request(Client(addresses, port), phrase);
    std::cout << "Ответ от сервера: " << result << std::endl;

    while (1);
    

    return 0;
}