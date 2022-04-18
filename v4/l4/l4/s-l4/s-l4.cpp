#include <iostream>
#include <string>
#include <vector>

#include "Server.h"
#include "banlist.h"

using namespace std;



// Функция обработки логики
string logic(string res, sockaddr* ip, vector <userlist>* list, string* cred)
{
    // перемнная хранения ответа
    string req;

    // обсервим list по exp_in
    observe_exp_in(list);

    // пробегаемся по всем клиентам
    int ind = -1;
    for (int i = 0; i < list->size(); i++) {
        if (ip->sa_family == (list->at(i).ip)->sa_family && ip->sa_data == (list->at(i).ip)->sa_data) {
            ind = i;
            cout << "Клиент найден в базе" << endl;
            break;
        }
    }
    // если нет - записываем
    if (ind == -1) {
        ind = list->size();

        userlist newclient;
        newclient.ip = ip;
        newclient.count = 0;
        newclient.baned = false;
        time_t now = time(0);
        newclient.exp_in = now;

        list->push_back(newclient);

        cout << "Клиент не найден в базе. Записываем." << endl;

    }

    // проверка на бан
    if (list->at(ind).baned) {
        cout << "Клиент в бан-листе. В доступе отказано." << endl;

        req = "403";
        return req;
    }



    // сравниваем
    if (res == *cred) {
        cout << "Клиент предоставил правилую пару логин/пароль. Доступ разрешён" << endl;

        // сбрасываем кол-во попыток
        list->at(ind).count = 0;

        req = "All good!";
        return req;
    }
    else {
        list->at(ind).count += 1;

        cout << "Клиент предоставил неправилую пару логин/пароль. В доступе отказано." << endl;

        
        if (list->at(ind).count >= 3) {
            set_exp_in(list, &ind, 5);
            cout << "Кол-во попыток превысило 3. Клиент записан в бан-лист на 5 минут." << endl;

        }

        req = "403";
        return req;
    }

    // какая-то неизвестная ошибка если мы до сюда дошли
    cout << "Неизвестиная ошибка." << endl;

    req = "500";
    return req;
}




int main()
{
    setlocale(LC_ALL, "rus");


    // структура хранения пользователей с баном или претендентами на бан
    vector <userlist> list;

    // задаём данные для входа
    string cred = "login pass123";

    // запускаем сервер с функцикй обработки логигки logic
    startServer(logic, &list, &cred, "10.185.239.66", "27015");

    return 0;
}