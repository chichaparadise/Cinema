#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "FilmManager\filmManager.h"
#include "UserManager\UserManager.h"
#include "consoleUtills\consoleUtils.h"

using namespace std;

void AuthorizedMenu(FilmManager&, UserManager&, User&);
void UnauthorizedMenu(BusDbContext*, UserDbContext*, User*&);
bool Authorisation(UserDbContext*, User*&);
bool Registration(UserDbContext*, User*&);
void ShowBusDb(BusDbContext*, User*&);
void ShowUserDb(UserDbContext*, User*&);

Console console = Console();

int main()
{
    Configuration* config = new Configuration("configuration.txt");
    config->ReadObject();
    UserDbContext* userDbContext = new UserDbContext(config->UsersDbSrc.c_str());
    userDbContext->ReadObject();
    User* currentUser = nullptr;
    BusDbContext* busDbContext = new BusDbContext(config->BusDbSrc.c_str());
    busDbContext->ReadObject();
    while (true) {
        if (currentUser == nullptr) UnauthorizedMenu(busDbContext, userDbContext, currentUser);
        else AuthorizedMenu(busDbContext, userDbContext, currentUser);
    }
    return 0;
}

void AuthorizedMenu(BusDbContext* busDbContext, UserDbContext* userDbContext, User*& currentUser)
{
    while (true) {
        system("cls");
        uint8_t choice;
        bool stop = false;
        cout << "Добро рожаловть в Систему Учёта Городского Транспорта " << currentUser->Name << "\n1. Просмотреть транспорт\n2. Выход из учётной записи\n";
        if (currentUser->Attributes) cout << "3. Просмотреть пользователей" << endl;
        cout << "Esc - Выход" << endl;
        choice = _getch();
        if (choice == 27) exit(0);
        choice -= 48;
        switch (choice) {
        case 1:
            ShowBusDb(busDbContext, currentUser);
            stop = true;
            break;
        case 2:
            currentUser = nullptr;
            stop = true;
            break;
        case 3:
            if (currentUser->Attributes) {
                ShowUserDb(userDbContext, currentUser);
                stop = true;
            }
            break;
        default:
        {
            break;
        }
        if (stop) break;
    }
}

void UnauthorizedMenu(BusDbContext* busDbContext, UserDbContext* userDbContext, User*& currentUser)
{
    while (true) {
        system("cls");
        uint8_t choice;
        bool stop = false;
        cout << "Добро рожаловть в Систему Учёта Городского Транспорта\n1. Зарегистрироваться\n2. Авторизоваться\n3. Просмотреть транспорт\nEsc - Выход" << endl;
        choice = _getch();
        if (choice == 27) exit(0);
        choice -= 48;
        switch (choice) {
        case 1:
            stop = Registration(userDbContext, currentUser);
            break;
        case 2:
            stop = Authorisation(userDbContext, currentUser);
            break;
        case 3:
            ShowBusDb(busDbContext, currentUser);
            stop = true;
            break;
        default:
            break;
        }
        if (stop) break;
    }
}

bool Authorisation(UserDbContext* userDbContext, User*& currentUser)
{
    system("cls");
    cout << "Введите имя" << endl;
    string name, password;
    cin >> name;
    cout << "Введите пароль" << endl;
    cin >> password;
    for (auto user : userDbContext->Users)
    {
        string tmpLog = user->Name;
        string tmpPass = user->Password;
        if (tmpLog == name && tmpPass == password)
        {
            currentUser = user;
            return true;
        }
    }
    cout << "Не успешно\nНажмите любую кнопку для выхода" << endl;
    _getch();
    return true;
}

bool Registration(UserDbContext* userDbContext, User*& user)
{
    system("cls");
    user = new User();
    cout << "Введите имя" << endl;
    string str;
    cin >> str;
    for (auto user : userDbContext->Users)
    {
        if (str == user->Name)
        {
            cout << "Пользователь с таким именем уже существует\nНажмите любую кнопку для выхода";
            _getch();
            return false;
        }
    }
    user->Name = str;
    cout << "Введите пароль" << endl;
    cin >> str;
    user->Password = str;
    if (userDbContext->Users.size() == 0) user->Attributes = true;
    else user->Attributes = false;
    user->Initialized = true;
    userDbContext->Users.push_back(user);
    userDbContext->UserCount++;
    userDbContext->WriteObject();
    return true;
}

void CreateBus(Bus*& bus)
{
    system("cls");
    if (bus == nullptr) bus = new Bus();
    cout << "Введите номер автобуса\n" << endl;
    cin >> bus->BusNumber;
    cout << "Введите количество станций автобуса\n" << endl;
    cin >> bus->BusStationCount;
    for (int i = 0; i < bus->BusStationCount; i++)
    {
        std::string str;
        cin >> str;
        bus->BusStations.push_back(str);
    }
    cout << "Введите время начала работы автобуса автобуса\n" << endl;
    cin >> bus->StartTime;
    cout << "Введите интервал работы автобуса\n" << endl;
    cin >> bus->dTime;
}

void ShowBusDb(BusDbContext* busDbContext, User*& user)
{
    system("cls");
    if (busDbContext != nullptr)
    {
        Bus* current = nullptr;
        int choice;
        while (true)
        {
            for (auto bus : busDbContext->Buses)
            {
                cout << "Номер автобуса: " << bus->BusNumber << "\n\t" << "Отсановки: " << bus->BusStationCount << endl;
                for (auto station : bus->BusStations) cout << "\t" << station << endl;
                cout << "\tВремя начала работы: " << bus->StartTime << endl << "Интервал работы: " << bus->dTime << endl;
            }
            cout << "\nВыход - esc" << endl;
            if (user != nullptr)
                if (user->Attributes)
                {
                    cout << endl << endl << "Изменить данные об автобусе - 1\nДобавить автобус - 2\nУдалить автобус - 3" << endl;
                }
            choice = _getch();
            if (choice == 27) return;
            if (user != nullptr)
                if (user->Attributes)
                {
                    choice -= 48;
                    if (choice > 3 || choice < 1) break;
                    system("cls");
                    switch (choice)
                    {
                    case 1:
                        while (true)
                        {
                            cout << "Автобусы:" << endl;
                            int counter = 0;
                            for (auto bus : busDbContext->Buses)
                            {
                                cout << ++counter << ". " << bus->BusNumber << endl;
                            }
                            cout << endl << "0 - выход" << endl;
                            int a;
                            cin >> a;
                            if (a == 0) return;
                            auto bus_it = busDbContext->Buses.begin();
                            for (int i = 0; i < a - 1; i++)
                            {
                                bus_it++;
                            }
                            CreateBus(current);
                            if ((*bus_it) != nullptr)
                            {
                                *bus_it = current;
                            }
                            busDbContext->WriteObject();
                            break;
                        }
                        ShowBusDb(busDbContext, user);
                        return;
                    case 2:
                        CreateBus(current);
                        busDbContext->Buses.push_back(current);
                        busDbContext->BusCount++;
                        busDbContext->WriteObject();
                        return;
                    case 3:
                        cout << "Автобусы:" << endl;
                        int counter = 0;
                        for (auto bus : busDbContext->Buses)
                        {
                            cout << ++counter << ". " << bus->BusNumber << endl;
                        }
                        cout << endl << "0 - выход" << endl;
                        int a;
                        cin >> a;
                        if (a == 0) return;
                        auto bus_it = busDbContext->Buses.begin();
                        for (int i = 0; i < a - 1; i++)
                        {
                            bus_it++;
                        }
                        if (*(bus_it) != nullptr) busDbContext->Buses.erase(bus_it);
                        busDbContext->BusCount--;
                        busDbContext->WriteObject();
                        return;
                    }
                }
        }
    }
    else {
        cout << "База данных не найдена\nEsc - выход" << endl;
        int a = _getch();
        if (a == 27) return;
    }
}

void ShowUserDb(UserDbContext* userDbContext, User*& currentUser)
{
    system("cls");
    if (userDbContext != nullptr)
    {
        int choice;
        while (true)
        {
            for (auto user : userDbContext->Users)
            {
                cout << "Имя пользователя: " << user->Name << "\n\t" << "Роль: ";
                if (user->Attributes) cout << "\t" << "админ";
                else cout << "Пользователь";
            }
            cout << "\nВыход - esc" << endl;
            if (currentUser != nullptr)
                if (currentUser->Attributes)
                {
                    cout << endl << endl << "Назначить Админа - 1\nЛишить админки - 2\nУдалить Пользователя - 3" << endl;
                }
            choice = _getch();
            if (choice == 27) return;
            if (currentUser != nullptr)
                if (currentUser->Attributes)
                {
                    choice -= 48;
                    if (choice > 3 || choice < 1) break;
                    system("cls");
                    switch (choice)
                    {
                    case 1:
                        while (true)
                        {
                            cout << "Пользователи:" << endl;
                            int counter = 0;
                            for (auto user : userDbContext->Users)
                            {
                                cout << ++counter << ". " << user->Name << endl;
                            }
                            cout << endl << "0 - выход" << endl;
                            int a;
                            cin >> a;
                            if (a == 0) return;
                            auto user_it = userDbContext->Users.begin();
                            for (int i = 0; i < a - 1; i++)
                            {
                                user_it++;
                            }
                            (*(user_it))->Attributes = true;
                            userDbContext->WriteObject();
                            break;
                        }
                        ShowUserDb(userDbContext, currentUser);
                        return;
                    case 2:
                        while (true)
                        {
                            cout << "Пользователи:" << endl;
                            int counter = 0;
                            for (auto user : userDbContext->Users)
                            {
                                cout << ++counter << ". " << user->Name << endl;
                            }
                            cout << endl << "0 - выход" << endl;
                            int a;
                            cin >> a;
                            if (a == 0) return;
                            auto user_it = userDbContext->Users.begin();
                            for (int i = 0; i < a - 1; i++)
                            {
                                user_it++;
                            }
                            (*(user_it))->Attributes = false;
                            userDbContext->WriteObject();
                            break;
                        }
                        ShowUserDb(userDbContext, currentUser);
                        return;
                    case 3:
                        while (true)
                        {
                            cout << "Пользователи:" << endl;
                            int counter = 0;
                            for (auto user : userDbContext->Users)
                            {
                                cout << ++counter << ". " << user->Name << endl;
                            }
                            cout << endl << "0 - выход" << endl;
                            int a;
                            cin >> a;
                            if (a == 0) return;
                            auto user_it = userDbContext->Users.begin();
                            for (int i = 0; i < a - 1; i++)
                            {
                                user_it++;
                            }
                            userDbContext->Users.erase(user_it);
                            userDbContext->UserCount--;
                            userDbContext->WriteObject();
                            break;
                        }
                        ShowUserDb(userDbContext, currentUser);
                        return;
                    }
                }
        }
    }
    else {
        cout << "База данных не найдена\nEsc - выход" << endl;
        int a = _getch();
        if (a == 27) return;
    }
}
