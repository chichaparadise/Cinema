#include <iostream>
#include <regex>

std::regex fioPattern_eng("^([A-Z])([a-z]*)\\s([A-Z])([a-z]*)$");
std::regex fioPattern_rus("^[А-Я]([а-я]*)\\s[А-Я]([а-я]*)$");

#include "stdio.h"
#include <conio.h>
#include <Windows.h>
#include <locale>
#include <clocale>
#include "FilmManager\filmManager.h"
#include "UserManager\UserManager.h"
#include "consoleUtills\consoleUtils.h"
#include "menu.h"
#include "Crypto\cryptoUtil.h"


using namespace std;


FilmManager filmManager;
UserManager userManager;
Console console; //Еьлан, мы чары кидаем

void ShowTickets(int);
list<Film> FindFilm();
void ShowUsers();
void SelectUser(int);
void ShowFilms();
void SelectFilm(int);
void Authorisation();
void Registration();
void BuyTicket();
void GetTicketOnFilm(int);
void Start();
void CreateFilm();
int mod(int, int);

static void DbSyncAtexit()
{
	userManager.WriteObject();
	userManager.filmManager.WriteObject();
}

int main()
{
	atexit(DbSyncAtexit);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	std::setlocale(LC_ALL, ""); // for C and C++ where synced with stdio
	std::locale::global(std::locale("")); // for C++
	std::cout.imbue(std::locale());
	filmManager = FilmManager("films.dat");
	userManager = UserManager("users.dat", filmManager);
	userManager.filmManager.SortFilms(Film::cmpByRoom);
    //Menu menu();
	while (true) Start();
    return 0;
}

int mod(int a, int b)
{
	int d = a / b;
	return abs(a) - abs(d) * b;
}

string GetPassword()
{
	string pass;
	int ch = 0;               // Переменная для символа.
	while (true)                // Создание бесконечного цикла.
	{
		ch = _getch();        // Помещаем код нажатой клавиши в переменную.
		if (ch == 13)         // Установка Enter на прерывание цикла.
		{
			break;         // Прерывание цикла.
		}
		if (ch == 27)   // Установка Esc на закрытие консоли.
		{
			exit(0);      // Выход из консоли.
		}
		if (ch == 8)     // Установка Backspace на удаление символов.
		{

			cout << (char)8 << ' ' << char(8);
			/*Смещение курсора на одну позицию  в лево вывод пробела и
			снова смещение курсора влево, то есть при нажатии Backspace
			символ будет стираться, а курсор перемещаеться. */

			if (!pass.empty())
				/*Если строка pass не являеться пустой, то из неё
				можно удалять  последний символ (Иначе закрывалась консоль.)*/

				pass.erase(pass.length() - 1);
			// позволяет удалить последний символ из строки pass
		}

		else
		{
			cout << '*';            // Замена символов на *
			pass += (char)ch;       // Преврашение кода из целого числа в символ.
		}
	}
	return pass;
}

void Start()
{
	system("cls");
	if (userManager.currentUser != nullptr)
	{
		int perms;
		if (userManager.currentUser->hasAttributes(UserAttributes::Admin))
		{
			perms = 4;
			printf("Добро пожаловать, %s!\n\tВыйти из учётной записи\n\tКупить билет\n\tРедактировать ,базу пользователей\n\tРедактировать базу фильмов\n\n\tEsc - выход", userManager.currentUser->GetName().c_str());
		}
		else
		{
			perms = 2;
			printf("Добро пожаловать, %s!\n\tВыйти из учётной записи\n\tКупить билет\n\n\tEsc - выход", userManager.currentUser->GetName().c_str());
		}
		int position = 0;
		while (_kbhit() == 0)
		{

		}
		char ch;
		while (true)
		{
			ch = _getch();
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
					if (userManager.currentUser->hasAttributes(UserAttributes::Admin)) ShowFilms();
					return;
				default:
					throw_exception("Unexpected cursor position", NULL, error_type::unknown_error, NULL);
				}
				break;
			case Keys::Esc:
				exit(0);
			default:
				break;
			}
			for (int i = 0; i < console.ySize; i++)
			{
				if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
				else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
			}
		}
	}
	else
	{
		printf("Добро пожаловать!\n\tЗарегистрироваться\n\tАвторизоваться\n\n\tEsc - выход");
		int position = 0;
		while (_kbhit() == 0)
		{

		}
		char ch;
		while (true)
		{
			ch = _getch();
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
			case Keys::Esc:
				exit(0);
			default:
				break;
			}
			for (int i = 0; i < console.ySize; i++)
			{
				if(i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
				else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
			}
		}
	}
}

void Authorisation()
{
		system("cls");
		printf("Введите имя пользователя:\n");
		string name;
		getline(cin, name);
		printf("Введите пароль:\n");
		string password = GetPassword();
		//getline(cin, password);
		if (!userManager.Authorise(name, password))
		{
			printf("Неверные данные\n\nEsc - возврат");
			if (_getch() == Keys::Esc) return;
		}
}

void Registration()
{
	Validator val;
	system("cls");
	bool isValid = false;
	string name, password;
	while (true) {
		printf("Введите имя пользователя:\n");
		getline(cin, name);
		isValid = val.isNameValid(name);
		if (!isValid) printf("Введенные данные не корректны\n\n");
		else break;
	}
	isValid = false;
	printf("Введите пароль:\n");
	password = GetPassword();
	crypto::xorEncrypt(password, crypto::_key);
	list<int> films;
	User user;
	if(userManager.userList.size() == 0) user = User(name, password, UserAttributes::Admin, films);
	else user = User(name, password, NULL, films);
	if (userManager.AddUser(user) == true)
	{
		userManager.currentUser = &*userManager.userList.rbegin();
		userManager.WriteObject();
	}
	else Registration();
}

void BuyTicket()
{
	new_films:
	system("cls");
	printf("Доступные фильмы():");
	list<Film> films_ = userManager.filmManager.GetFilms();
	for (auto film : films_)
	{
		printf("\n\t%s", film.name.c_str());
	}
	printf("\n\n\tEsc - выход\nF - найти фильмы\nS - сортировать по цене");
	int position = 0;
	auto it = films_.begin();
	while (_kbhit() == 0)
	{

	}
	char ch;
	while (true)
	{
		ch = _getch();
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(--position, films_.size());
			break;
		case Keys::DownArrow:
			position = mod(++position, films_.size());
			break;
		case Keys::Enter:
			for (int pos = 0; it != films_.end() && pos < position; ++it, ++pos);
			GetTicketOnFilm((*it).FilmId);
			return;
		case Keys::Esc:
			return;
		case 'F':
			films_ = FindFilm();
			goto new_films;
		case 'А':
			films_ = FindFilm();
			goto new_films;
		case 'S':
			userManager.filmManager.SortFilms(Film::cmpByTicketCost);
			films_ = userManager.filmManager.GetFilms();
			goto new_films;
		case 'Ы':
			userManager.filmManager.SortFilms(Film::cmpByTicketCost);
			films_ = userManager.filmManager.GetFilms();
			goto new_films;
		default:
			break;
		}
		for (int i = 0; i < console.ySize; i++)
		{
			if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
}

void GetTicketOnFilm(int id)
{
	system("cls");
	int k = -1;
	Film setFilm = userManager.filmManager.SearchFilmById(id);
	printf("Название:%s\n\tID:%d\n\tДата:%s\n\tВремя:%s\n\tЦена:%d руб\n\tЗал:%d\n\t", setFilm.name.c_str(), setFilm.FilmId, setFilm.data.c_str(), setFilm.time.c_str(), setFilm.cost, setFilm.room);
	printf("Купить билет\n\nEsc - выход");
	while (_kbhit() == 0)
	{

	}
	char ch = _getch();
	for (int i = 0; i < console.ySize; i++)
	{
		if (i == 6)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
		else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
	}
	while (true)
	{
		switch (ch = _getch())
		{
		case Keys::Enter:
			if (userManager.filmManager.BuyTicketById(setFilm.FilmId))
			{
				userManager.currentUser->AddFilm(setFilm.FilmId);
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
}

void ShowUsers()
{
	system("cls");
	printf("Имя пользователя:");
	for (auto user : userManager.userList)
	{
		printf("\n\t%s", user.GetName().c_str());
	}
	printf("\n\nDelete - удалить\nEsc - выход");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch;
	while (true)
	{
		ch = _getch();
		auto user_it = userManager.userList.begin();
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(--position, (int)userManager.userList.size());
			break;
		case Keys::DownArrow:
			position = mod(++position, (int)userManager.userList.size());
			break;
		case Keys::Delete:
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
		case Keys::Esc:
			return;
		}
		for (int i = 0; i < console.ySize; i++)
		{
			if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
}

void SelectUser(int pos)
{
	Validator val;
	system("cls");
	auto user_it = userManager.userList.begin();
	for (int i = 0; i < pos; i++)
	{
		user_it++;
	}
	printf("Имя пользователя:\n %s\n", (*user_it).GetName().c_str());
	if ((*user_it).hasAttributes(UserAttributes::Admin))
		printf("Admin\n");
	else
		if ((*user_it).hasAttributes(UserAttributes::Blocked))
			printf("Заблокирован\n");
		else printf("Пользователь\n");
	printf("Фильмы:");
	for (auto filmID : (*user_it).GetFilms()) {
		auto film = userManager.filmManager.SearchFilmById(filmID);
		printf("\n\tНазвание:%s\n\t\tID:%d\n\t\tДата:%s\n\t\tВремя:%s\n\t\tЦена:%d руб\n\t\tЗал:%d", film.name.c_str(), film.FilmId, film.data.c_str(), film.time.c_str(), film.cost, film.room);
	}
	printf("\n\nEsc - выход");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch;
	while (true)
	{
		ch = _getch();
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(--position, 3);
			break;
		case Keys::DownArrow:
			position = mod(++position, 3);
			break;
		case Keys::Esc:
			return;
		case Keys::Enter:
			switch (position)
			{
			case 0:
				system("cls");
				if ((*user_it).GetName() != userManager.currentUser->GetName())
				{
					printf("Вы не можете это сделать\n\nEsc - выход");
					while (_getch() != Keys::Esc);
					return;
				}
				while (true)
				{
					printf("Введите новое имя:\n");
					string name, password;
					getline(cin, name);
					if (val.isNameValid(name))
					{
						printf("Подтвердите пароль:\n");
						password = GetPassword();
						if ((*user_it).SetName(name, password))
						{
							userManager.WriteObject();
							return;
						}
						else
						{
							printf("Не успешно\n\nEsc - выход");
							while (_getch() != Keys::Esc);
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
					char ch;
					while (true)
					{
						ch = _getch();
						switch (ch)
						{
						case Keys::UpArrow:
							position = mod(--position, 2);
							break;
						case Keys::DownArrow:
							position = mod(++position, 2);
							break;
						case Keys::Esc:
							return;
						case Keys::Enter:
							string password;
							printf("Подтвердите пароль:\n");
							password = GetPassword();
							bool confirmed = userManager.currentUser->IsPasswordValid(password);
							if (position == 0 && confirmed)
								if (userManager.SetAttributes(*user_it, UserAttributes::Admin))
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
							if (position == 1 && confirmed)
								if ((*user_it).hasAttributes(UserAttributes::Blocked))
								{
									if (userManager.SetAttributes(*user_it, NULL))
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
									if (userManager.SetAttributes(*user_it, UserAttributes::Blocked))
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
						for (int i = 0; i < console.ySize; i++)
						{
							if (i == position)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
							else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
						}
					}
				}
			case 2:
				system("cls");
				printf("Фильмы:");
				for (auto filmID : (*user_it).GetFilms()) {
					auto film = userManager.filmManager.SearchFilmById(filmID);
					printf("\n%s", film.name.c_str());
				}
				printf("\nEnter - удалить\n\nEsc - выход");
					int position = 0;
					while (_kbhit() == 0)
					{
						;
					}
					char ch;
					while (true)
					{
						ch = _getch();
						switch (ch)
						{
						case Keys::UpArrow:
							position = mod(--position, (int)(*user_it).GetFilms().size());
							return;
						case Keys::DownArrow:
							position = mod(++position, (int)(*user_it).GetFilms().size());
							return;
						case Keys::Esc:
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
						for (int i = 0; i < console.ySize; i++)
						{
							if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
							else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
						}
					}
			}
			return;
		default:
			break;
		}
		for (int i = 0; i < console.ySize; i++)
		{
			if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
}

void ShowFilms()
{
	system("cls");
	printf("Название фильма:");
	for (auto film : userManager.filmManager.GetFilms())
	{
		printf("\n\t%s", film.name.c_str());
	}
	printf("\n\nDelete - удалить\nEsc - выход\nA - добавить фильм\nF - найти фильм\nEnter - выбрать фильм");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch = _getch();
	while (true)
	{
		auto film_it = userManager.filmManager.GetFilms().begin();
		ch = _getch();
		switch (ch)
		{
		case Keys::UpArrow:
			position = (int)userManager.filmManager.GetFilms().size() != 0 ? mod(--position, (int)userManager.filmManager.GetFilms().size()) : 0;
			break;
		case Keys::DownArrow:
			position = (int)userManager.filmManager.GetFilms().size() != 0 ? mod(++position, (int)userManager.filmManager.GetFilms().size()) : 0;
			break;
		case Keys::Delete:
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
		case 'A':
			CreateFilm();
			return;
		case 'ф':
			CreateFilm();
			return;
		case 'F':
			FindFilm();
			return;
		case 'А':
			FindFilm();
			return;
		case Keys::Esc:
			return;
		default:
			break;
		}
		//ch = _getch();
		for (int i = 0; i < console.ySize; i++)
		{
			if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
}

list<Film> FindFilm()
{
	list<Film> responce;
	system("cls");
	printf("Найти по названию\nНайти по Id\nНайти по дате\n\nEsc - выход");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch;
	bool isSet = false;
	while (!isSet)
	{
		ch = _getch();
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(--position, 3);
			break;
		case Keys::DownArrow:
			position = mod(++position, 3);
			break;
		case Keys::Enter:
			isSet = true;
			break;
		case Keys::Esc:
			return userManager.filmManager.GetFilms();
		default:
			break;
		}
		for (int i = 0; i < console.ySize; i++)
		{
			if (i == position)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
	string pattern;
	system("cls");
	if (position == 0)
	{
		printf("Введите название: ");
		getline(cin, pattern);
		list<Film> results = userManager.filmManager.SearchFilmsByName(pattern);
		printf("Фильмов найдено: %d", results.size());
		for (auto film : results)
		{
			printf("Фильм: %s \t Id: %d", film.name.c_str(), film.FilmId);
		}
		responce = results;
	}
	else if (position == 1)
	{
		printf("Введите id: ");
		getline(cin, pattern);
		while (Validator::useRegex(pattern, "^[0-9]{1,8}$"))
		{
			printf("\nВведите id: ");
			getline(cin, pattern);
		}
		int id = atoi(pattern.c_str());
		Film result = userManager.filmManager.SearchFilmById(id);
		printf("Фильм: %s \t Id: %d", result.name.c_str(), result.FilmId);
		responce.push_front(result);
	}
	else
	{
		printf("Введите дату: ");
		getline(cin, pattern);
		while (!Validator::isDataValid(pattern))
		{
			printf("\nВведите дату: ");
			getline(cin, pattern);
		}
		list<Film> results = userManager.filmManager.SearchFilmsByDate(pattern);
		for (auto film : results)
		{
			printf("Фильм: %s \t Id: %d", film.name.c_str(), film.FilmId);
		}
		responce = results;
	}
	printf("\n\nEsc - выход: ");
	while (char h = _getch() != Keys::Esc);
	return responce;
}

void CreateFilm()
{
	system("cls");
	Film* film;
	string title;
	printf("Set film title: ");
	getline(cin, title);
	string date;
	printf("\nSet show date (dd.mm.yyyy): ");
	getline(cin, date);
	while (!Validator::isDataValid(date))
	{
		printf("\nWrong format try again, set show date (dd.mm.yyyy): ");
		getline(cin, date);
	}
	string time;
	printf("\nSet show time (hh:mm): ");
	getline(cin, time);
	while (!Validator::isTimeValid(time))
	{
		printf("\nWrong format try again, set show time (hh:mm): ");
		getline(cin, time);
	}
	uint8_t _room;
	string room;
	printf("\nSet room [0-99]: ");
	getline(cin, room);
	while (!Validator::useRegex(room, "^[0-9]{1,2}$"))
	{
		printf("\nWrong format try again, set room [0-255]: ");
		getline(cin, room);
	}
	_room = (uint8_t)atoi(room.c_str());
	uint32_t cost;
	string _cost;
	printf("\nSet ticket cost [0-99999]: ");
	getline(cin, _cost);
	while (!Validator::useRegex(_cost, "^[0-9]{1,5}$"))
	{
		printf("\nWrong format try again, set ticket cost [0-99999]: ");
		getline(cin, _cost);
	}
	cost = (uint8_t)atoi(_cost.c_str());
	int FilmId = userManager.filmManager.GetNewId();
	film = new Film(FilmId, title, date, time, _room, cost);
	userManager.filmManager.AddFilm(*film);
	userManager.filmManager.WriteObject();
	delete film;
	return;
}

void ShowTickets(int filmId)
{
	bool isBought = false;
	uint8_t Attribute;
	char* buff;
	Film film = userManager.filmManager.SearchFilmById(filmId);
	buff = userManager.filmManager.ReturnFilmPlaces(film);
	for (int row = 0; row < ROOM_ROWS; ++row)
	{
		printf("\t");
		for (int col = 0; col < ROOM_PLACES_IN_A_ROW; ++col)
		{
			isBought = *(buff++) == EMPTY_PLACE ? false : true;
			if (isBought)
				console.SetAttribute(ConsoleColor::Red, ConsoleColor::Red);
			else
				console.SetAttribute(ConsoleColor::Green, ConsoleColor::Green);
			printf(" ");
			console.SetAttribute(ConsoleColor::Black, ConsoleColor::White);
			printf(" ");
		}
		printf("\n\n");
	}
}

void SelectFilm(int pos)
{
	system("cls");
	int k = -1;
	Film setFilm;
	for (auto film : userManager.filmManager.GetFilms())
	{
		k++;
		if (k >= pos)
		{
			setFilm = film;
			break;
		}
	}
	printf("Фильмы:");
	printf("\n\tНазвание:%s\n\t\tID:%d\n\t\tДата:%s\n\t\tВремя:%s\n\t\tЦена:%d руб\n\t\tЗал:%d\n\t\tСуммарный сбор:%d\n\nМеста:\n\n", setFilm.name.c_str(), setFilm.FilmId, setFilm.data.c_str(), setFilm.time.c_str(), setFilm.cost, setFilm.room, setFilm.totalCost);
	ShowTickets(setFilm.FilmId);
	printf("\n\nEsc - выход");
	int position = 0;
	while (_kbhit() == 0)
	{

	}
	char ch;
	while (true)
	{
		ch = _getch();
		switch (ch)
		{
		case Keys::UpArrow:
			position = mod(--position, 5);
			break;
		case Keys::DownArrow:
			position = mod(++position, 5);
			break;
		case Keys::Esc:
			return;
		case Keys::Enter:
			string name;
			string date;
			string time;
			switch (position)
			{
			case 0:
				system("cls");
				printf("Введите новое название:\n");
				getline(cin, name);
				userManager.filmManager.SearchFilmById_unsafe(setFilm.FilmId).name = name;
				userManager.filmManager.WriteObject();
				return;
			case 1:
				system("cls");
				printf("Нельзя менять ID");
				return;
			case 2:
				system("cls");
				while (true)
				{
					printf("Введите новую дату:\n");
					getline(cin, date);
					if (Validator::isDataValid(date))
					{
						userManager.filmManager.SearchFilmById_unsafe(setFilm.FilmId).data = date;
						userManager.filmManager.WriteObject();
						return;
					}
					printf("Неверный формат\n\n");
				}
			case 3:
				system("cls");
				while (true)
				{
					printf("Введите новое время:\n");
					getline(cin, time);
					if (Validator::isTimeValid(time))
					{
						userManager.filmManager.SearchFilmById_unsafe(setFilm.FilmId).time = time;
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
					uint32_t cost;
					string _cost;
					printf("\nSet ticket cost [0-99999]: ");
					getline(cin, _cost);
					while (!Validator::useRegex(_cost, "^[0-9]{1,5}$"))
					{
						printf("\nWrong format try again, set ticket cost [0-99999]: ");
						getline(cin, _cost);
					}
					cost = atoi(_cost.c_str());
					userManager.filmManager.SearchFilmById_unsafe(setFilm.FilmId).time = time;
					userManager.filmManager.WriteObject();
					return;
				}
			}
		}
		for (int i = 0; i < 6; i++)
		{
			if (i == position + 1)console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Magenta, ConsoleColor::Cyan);
			else console.DrawAttributeHorizontalLine(0, i, console.xSize, ConsoleColor::Black, ConsoleColor::White);
		}
	}
}