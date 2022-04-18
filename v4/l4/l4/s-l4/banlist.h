#pragma once

#include <string>
#include <winsock2.h>
#include "ctime"
#include <vector>

using namespace std;

struct userlist {
	sockaddr* ip;
	int count;
	time_t exp_in;
	bool baned;
};

void observe_exp_in(vector <userlist>* list);
void set_exp_in(vector <userlist>* list, int* ind, int ban_time);