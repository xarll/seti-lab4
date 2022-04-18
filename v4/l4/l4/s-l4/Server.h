#include <iostream>
#include <vector>
#include "banlist.h"

using namespace std;

int startServer(string(*func)(string res, sockaddr* ip, vector <userlist>* list, string* cred), vector <userlist>* list, string* cred, const char* addresses, const char* port);