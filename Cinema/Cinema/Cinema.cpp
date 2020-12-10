#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "FilmManager\filmManager.h"
#include "UserManager\UserManager.h"
#include "consoleUtills\consoleUtils.h"
#include "menu.h"
#include "Crypto\cryptoUtil.h"

using namespace std;

FilmManager filmManager("films.dat");
UserManager userManager("user.dat", filmManager);
Console console(600, 400);

int main()
{
    Menu menu();
	while (true) Start();
    return 0;
}

int mod(int a, int b)
{
	int d = a / b;
	return abs(a) - abs(d) * b;
}

void Start()
{
	if (userManager.currentUser != nullptr)
	{
		int perms;
		if (userManager.currentUser->hasAttributes(UserAttributes::Admin))
		{
			perms = 4;
			printf("Добро пожаловать, %s!\n\tВыйти из учётной записи\n\tКупить билет\n\tРедактировать ,базу пользователей\n\tРедактировать базу фильмов\n\n\tEsc - выход", userManager.currentUser->GetName());
		}
		else
		{
			perms = 2;
			printf("Добро пожаловать, %s!\n\tВыйти из учётной записи\n\tКупить билет\n\n\tEsc - выход", userManager.currentUser->GetName());
		}
		int position = 0;
		while (_kbhit() == 0)
		{

		}
		char ch = _getch();
		while (ch != Keys::Esc)
		{
			switch (ch)
			{
			case Keys::UpArrow:
				position = mod(--position, perms);
				break;
			case Keys::DownArrow:
				position = mod(++position, perms);
				break;
			case Keys::Enter:
				switch (position)
				{
				case 0:
					userManager.Logout();
					return;
				case 1:
					BuyTicket();
					return;
				case 2:
					if(userManager.currentUser->hasAttributes(UserAttributes::Admin) ShowUsers();
					return;
				case 3:
					if (userManager.currentUser->hasAttributes(UserAttributes::Admin) ShowFilms();
					return;
				default:
					throw_exception("Unexpected cursor position", NULL, error_type::unknown_error, NULL);
				}
				break;
			default:
				break;
			}
			console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
		}
	}
	else
	{
		printf("Добро пожаловать!\n\tЗарегистрироваться\n\tАвторизоваться\n\n\tEsc - выход");
		int position = 0;
		while (_kbhit() == 0)
		{

		}
		char ch = _getch();
		while (ch != Keys::Esc)
		{
			switch (ch)
			{
			case Keys::UpArrow:
				position = mod(--position, 2);
				break;
			case Keys::DownArrow:
				position = mod(++position, 2);
				break;
			case Keys::Enter:
				switch (position)
				{
				case 0:
					Registration();
					return;
				case 1:
					Authorisation();
					return;
				default:
					throw_exception("Unexpected cursor position", NULL, error_type::unknown_error, NULL);
				}
				break;
			default:
				break;
			}
			console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
		}
	}
}

void Authorisation()
{
		system("cls");
		printf("Введите имя пользователя:\n");
		string name;
		::cin >> name;
		printf("Введите пароль:\n");
		string password;
		::cin >> password;
		if (!userManager.Authorise(name, password))
		{
			printf("Неверные данные\n\nEsc - возврат");
			if (_getch() == Keys::Esc) return;
		}
}

void Registration()
{
	system("cls");
	bool isValid = false;
	string name, password;
	while (true) {
		printf("Введите имя пользователя:\n");
		::cin >> name;
		isValid = Validator::isNameValid(name);
		if (!isValid) printf("Введенные данные не корректны\n");
		else break;
	}
	isValid = false;
	printf("Введите пароль:\n");
	::cin >> password;
	crypto::xorEncrypt(password, crypto::_key);
	list<int> films;
	User user;
	if(userManager.userList.size() == 0) user = User(name, password, UserAttributes::Admin, films);
	else user = User(name, password, NULL, films);
	userManager.AddUser(user);
	userManager.currentUser = &user;
	userManager.WriteObject();
}

void BuyTicket()
{
	system("cls");
	printf("Доступные фильмы():");
	for (auto film : userManager.filmManager.GetFilms())
	{
		printf("\n\tНазвание:%s\n\t\tID:%d\n\t\tДата:%s\n\t\tВремя:%s\n\t\tЦена:%d руб\n\t\t", film.name, film.FilmId, film.data, film.time, film.cost, film.room);
	}
	printf("\n\n\tEsc - выход");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch = _getch();
	while (ch != Keys::Esc)
	{
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(position - 5, userManager.filmManager.GetFilms().size()*5);
			break;
		case Keys::DownArrow:
			position = mod(position + 5, userManager.filmManager.GetFilms().size()*5);
			break;
		case Keys::Enter:
			GetTicketOnFilm(position/=5);
			break;
		default:
			break;
		}
		for(int i=0; i<5; i++)console.DrawAttributeHorizontalLine(0, position + 1 + i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
	}
}

void GetTicketOnFilm(int position)
{
	system("cls");
	auto film_it = userManager.filmManager.GetFilms().begin();
	for (int i = 0; i < position; i++)
	{
		film_it++;
	}
	printf("Название:%s\n\tID:%d\n\tДата:%s\n\tВремя:%s\n\tЦена:%d руб\n\t", (*film_it).name, (*film_it).FilmId, (*film_it).data, (*film_it).time, (*film_it).cost, (*film_it).room);
	printf("Купить билет\n\nEsc - выход");
	while (_kbhit() == 0)
	{

	}
	char ch = _getch();
	switch (ch)
	{
	case Keys::Enter:
		if ((*film_it).BuyTicket())
		{
			userManager.currentUser->AddFilm((*film_it).FilmId);
			system("cls");
			printf("Успех\n\nEsc - выход");
			while (_getch() != Keys::Esc);
		}
		else
		{
			system("cls");
			printf("Сожалеем, билеты закончились\n\nEsc - выход");
			while (_getch() != Keys::Esc);
		}
	}
}