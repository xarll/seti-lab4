#include "banlist.h"

using namespace std;

void set_exp_in(vector <userlist>* list, int* ind, int ban_time) {

	// ������������ �����
	time_t now = time(0);
	now += ban_time * 60;

	// ���������� exp_in
	list->at(*ind).baned = true;
	list->at(*ind).exp_in = now;
}

void observe_exp_in(vector <userlist>* list) {

	// �������� ������� �����
	time_t now = time(0);

	// ����������� �� ���� ��������
	for (int i = 0; i < list->size(); i++) {
		if (now >= list->at(i).exp_in) {
			if (list->at(i).baned) {
				list->at(i).baned = false;
				list->at(i).count = 0;
			}
		}
	}
}