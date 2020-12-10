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

void ShowUsers();

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
					if(userManager.currentUser->hasAttributes(UserAttributes::Admin)) ShowUsers();
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
		printf("\n\t%s", film.name);
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
			position = mod(--position, userManager.filmManager.GetFilms().size());
			break;
		case Keys::DownArrow:
			position = mod(++position, userManager.filmManager.GetFilms().size());
			break;
		case Keys::Enter:
			GetTicketOnFilm(position);
			return;
		default:
			break;
		}
		console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
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
			return;
		}
		else
		{
			system("cls");
			printf("Сожалеем, билеты закончились\n\nEsc - выход");
			while (_getch() != Keys::Esc);
			return;
		}
	case Keys::Esc:
		return;
	}
}

void ShowUsers()
{
	system("cls");
	printf("Имя пользователя:");
	for (auto user : userManager.userList)
	{
		printf("\n\t%s", user.GetName());
	}
	printf("\n\nDelete - удалить\nEsc - выход");
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
			position = mod(--position, userManager.userList.size());
			break;
		case Keys::DownArrow:
			position = mod(++position, userManager.userList.size());
			break;
		case Keys::Delete:
			auto user_it = userManager.userList.begin();
			for (int i = 0; i < position; i++)
			{
				user_it++;
			}
			userManager.userList.erase(user_it);
			userManager.WriteObject();
			return;
		case Keys::Enter:
			SelectUser(position);
			return;
		default:
			break;
		}
		console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
	}
}

void SelectUser(int position)
{
	system("cls");
	auto user_it = userManager.userList.begin();
	for (int i = 0; i < position; i++)
	{
		user_it++;
	}
	printf("Имя пользователя\n", (*user_it).GetName());
	if ((*user_it).hasAttributes(UserAttributes::Admin))
		printf("Admin\n");
	else
		if ((*user_it).hasAttributes(UserAttributes::Blocked))
			printf("Заблокирован\n");
		else printf("Пользователь\n");
	printf("Фильмы:");
	for (auto filmID : (*user_it).GetFilms()) {
		auto film = userManager.filmManager.SearchFilmById(filmID);
		printf("\n\tНазвание:%s\n\t\tID:%d\n\t\tДата:%s\n\t\tВремя:%s\n\t\tЦена:%d руб", film.name, film.FilmId, film.data, film.time, film.cost, film.room); \
	}
	printf("\n\nEsc - выход");
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
			position = mod(--position, 3);
			break;
		case Keys::DownArrow:
			position = mod(++position, 3);
			break;
		case Keys::Enter:
			switch (position)
			{
			case 0:
				system("cls");
				if ((*user_it).GetName() != userManager.currentUser->GetName())
				{
					printf("Вы не можете это сделать\n\nEsc - выход");
					while (_getch() != 27);
					return;
				}
				while (true)
				{
					printf("Введите новое имя:\n");
					string name, password;
					::cin >> name;
					if (Validator::isNameValid(name))
					{
						printf("Подтвердите пароль:\n");
						::cin >> password;
						if ((*user_it).SetName(name, password))
						{
							userManager.WriteObject();
							return;
						}
						else
						{
							printf("Не успешно\n\nEsc - выход");
							while (_getch() != 27);
							return;
						}
					}
					else printf("Некорректные данные\n\n");
				}
			case 1:
				system("cls");
				if (!(*user_it).hasAttributes(UserAttributes::Admin))
				{
					printf("Повысить до Admin\nЗаблокировать/Разблокировать\n\nEsc - выход");
					int position = 0;
					while (_kbhit() == 0)
					{
						;
					}
					char ch = _getch();
					while (ch != 27)
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
							string password;
							printf("Подтвердите пароль:\n");
							::cin >> password;
							if (position == 0)
								if ((*user_it).SetAttributes(UserAttributes::Admin, password))
								{
									userManager.WriteObject();
									return;
								}
							else
							{
								printf("Не успешно\n\nEsc - выход");
								while (_getch() != 27);
								return;
							}
							if (position == 1)
								if ((*user_it).hasAttributes(UserAttributes::Blocked))
								{
									if ((*user_it).SetAttributes(NULL, password))
									{
										userManager.WriteObject();
										return;
									}
									else
									{
										printf("Не успешно\n\nEsc - выход");
										while (_getch() != 27);
										return;
									}
								}
								else
								{
									if ((*user_it).SetAttributes(UserAttributes::Blocked, password))
									{
										userManager.WriteObject();
										return;
									}
									else
									{
										printf("Не успешно\n\nEsc - выход");
										while (_getch() != 27);
										return;
									}
								}
						}
						console.DrawAttributeHorizontalLine(0, position, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
					}
				}
			case 2:
				system("cls");
				printf("Фильмы:");
				for (auto filmID : (*user_it).GetFilms()) {
					auto film = userManager.filmManager.SearchFilmById(filmID);
					printf("\n%s", film.name);
				}
				printf("\nEnter - удалить\n\nEsc - выход");
					int position = 0;
					while (_kbhit() == 0)
					{
						;
					}
					char ch = _getch();
					while (ch != 27)
					{
						switch (ch)
						{
						case Keys::UpArrow:
							position = mod(--position, (*user_it).GetFilms().size());
							return;
						case Keys::DownArrow:
							position = mod(++position, (*user_it).GetFilms().size());
							return;
						case Keys::Enter:
							auto film_it = (*user_it).GetFilms().begin();
							for (int i = 0; i < position; i++)
							{
								film_it++;
							}
							(*user_it).GetFilms().erase(film_it);
							userManager.WriteObject();
						}
						console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
					}
			}
			return;
		default:
			break;
		}
		console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
	}
}

void ShowFilms()
{
	system("cls");
	printf("Название фильма:");
	for (auto film : userManager.filmManager.GetFilms())
	{
		printf("\n\t%s", film.name);
	}
	printf("\n\nDelete - удалить\nEsc - выход");
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
			position = mod(--position, userManager.filmManager.GetFilms().size());
			break;
		case Keys::DownArrow:
			position = mod(++position, userManager.filmManager.GetFilms().size());
			break;
		case Keys::Delete:
			auto film_it = userManager.filmManager.GetFilms().begin();
			for (int i = 0; i < position; i++)
			{
				film_it++;
			}
			userManager.filmManager.GetFilms().erase(film_it);
			filmManager.WriteObject();
			return;
		case Keys::Enter:
			SelectFilm(position);
			return;
		default:
			break;
		}
		console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
	}
}

void SelectFilm(int position)
{
	system("cls");
	auto film_it = userManager.filmManager.GetFilms().begin();
	for (int i = 0; i < position; i++)
	{
		film_it++;
	}
	printf("Фильмы:");
	printf("\n\tНазвание:%s\n\t\tID:%d\n\t\tДата:%s\n\t\tВремя:%s\n\t\tЦена:%d руб", (*film_it).name, (*film_it).FilmId, (*film_it).data, (*film_it).time, (*film_it).cost, (*film_it).room);
	printf("\n\nEsc - выход");
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
			position = mod(--position, 5);
			break;
		case Keys::DownArrow:
			position = mod(++position, 5);
			break;
		case Keys::Enter:
			switch (position)
			{
			case 0:
				system("cls");
				string name;
				printf("Введите новое название:\n");
				::cin >> name;
				(*film_it).name = name;
				userManager.filmManager.WriteObject();
				return;
			case 1:
				system("cls");
				printf("Нельзя менять ID");
				return;
			case 2:
				system("cls");
				string date;
				while (true)
				{
					printf("Введите новую дату:\n");
					::cin >> name;
					if (Validator::isDataValid(date))
					{
						(*film_it).data = date;
						userManager.filmManager.WriteObject();
						return;
					}
					printf("Неверный формат\n\n");
				}
			case 3:
				system("cls");
				string time;
				while (true)
				{
					printf("Введите новое время:\n");
					::cin >> time;
					if (Validator::isTimeValid(time))
					{
						(*film_it).time = time;
						userManager.filmManager.WriteObject();
						return;
					}
					printf("Неверный формат\n\n");
				}
			case 4:
				system("cls");
				int cost;
				while (true)
				{
					printf("Введите новую цену:\n");
					::cin >> cost;
					(*film_it).cost = cost;
					userManager.filmManager.WriteObject();
					return;
				}
			}
		default:
			break;
		}
		console.DrawAttributeHorizontalLine(0, position + 1, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
	}
}