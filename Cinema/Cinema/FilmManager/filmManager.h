#ifndef FILM_MANAGER_H
#define FILM_MANAGER_H

#include <fstream>
#include <list>
#include "film.h"
#include "..\fileUtils\fileUtil.h"
#include "..\Crypto\cryptoUtil.h"
#include "..\Validator\validation.h"

#define FILMDBWRITE_EX 4
#define FILMDBREAD_EX 8
#define FILMMANAGMENT_EX 0x10

class FilmManager : base_entity
{
public:
    FilmManager() : base_entity((char*)"NULL")
    {
    }

    FilmManager(const string& path) : base_entity((char*)path.c_str())
    {
        this->ReadObject();
    }

    list<Film> SearchFilmsByName(string name_pattern)
    {
        list<Film> _Responce;
        for (auto film : this->filmList)
        {
            if (Validator::isSubstring(film.name, name_pattern))
                _Responce.push_back(film);
        }
        return _Responce;
    }

    list<Film> SearchFilmsByDate(string date)
    {
        list<Film> _Responce;
        if (Validator::isDataValid(date))
        {
            for (auto film : this->filmList)
            {
                if (film.data == date)
                    _Responce.push_back(film);
            }
        }
        return _Responce;
    }

    list<Film> GetFilms()
    {
        list<Film> newList(filmList);
        return newList;
    }

    Film SearchFilmById(int& filmId)
    {
        for (auto film : filmList)
            if (film.FilmId == filmId)
                return film;
        return Film();
    }

    Film& SearchFilmById_unsafe(int& filmId)
    {
        for (auto film_it = this->filmList.begin(); film_it != this->filmList.end(); ++film_it)
            if (filmId == (*film_it).FilmId)
                return *film_it;
    }

    void SortFilms(int (*_pred)(const Film&, const Film&))
    {
        this->filmList.sort(_pred);
    }

    inline char* ReturnFilmPlaces(const Film& film)
    {
        return (char*)film.tickets;
    }

    bool DeleteFilm(int& filmId)
    {
        for (auto film : filmList)
            if (film.FilmId == filmId)
            {
                this->filmList.remove(film);
                return true;
            }
        return false;
    }

    bool BuyTicketById(int& filmId)
    {
        for (auto film_it = this->filmList.begin(); film_it != this->filmList.end(); ++film_it)
            if (filmId == (*film_it).FilmId)
                return (*film_it).BuyTicket();
        return false;
    }

    bool AddFilm(Film& film)
    {
        film.FilmId = currentFilmId++;
        for (auto _film : filmList)
            if (film.FilmId == _film.FilmId)
                return false;
        filmList.push_back(film);
        return true;
    }

    bool ReadObject()
    {
        uint64_t sum = 0;
        uint64_t fileSum = -1;
        ifstream stream;
        stream.open(this->connection_string, ios::binary | ios::in);
        if (stream.is_open())
        {
            stream >> this->currentFilmId;
            sum += this->currentFilmId;
            int size;
            stream >> size;
            sum += size;
            for (int i = 0; i < size; ++i)
            {
                stream.ignore();
                char buffer[256];
                Film* film = new Film();
                stream.getline(buffer, 256, '\n');
                film->name = buffer;
                sum += crypto::stringSum(film->name);
                stream.getline(buffer, 256, '\n');
                film->data = buffer;
                sum += crypto::stringSum(film->data);
                stream.getline(buffer, 256, '\n');
                film->time = buffer;
                sum += crypto::stringSum(film->time);
                stream >> film->tickets;
                sum += crypto::stringSum(reinterpret_cast<char*>(film->tickets));
                stream >> film->FilmId;
                sum += film->FilmId;
                stream >> film->room;
                sum += film->room;
                stream >> film->cost;
                sum += film->cost;
                stream >> film->totalCost;   
                sum += film->totalCost;
                this->filmList.push_back(*film);
                delete film;
            }
            stream >> fileSum;
            stream.close();
            if (sum == fileSum)
                return true;
            throw_exception("file open exception, try to resolve exception", FilmManager::FilmManagerHandler, FILMDBREAD_EX, (void*)this);
            return false;
        }
        throw_exception("file open exception, terminating process", FilmManager::FilmManagerHandler, FILMDBREAD_EX | error_type::critical_error, (void*)this);
        stream.close();
        return false;
    }

    bool WriteObject()
    {
        uint64_t sum = 0;
        ofstream stream;
        stream.open(this->connection_string, ios::binary | ios::trunc | ios::out);
        if (stream.is_open())
        {
            sum += currentFilmId;
            stream << currentFilmId << endl;
            sum += this->filmList.size();
            stream << this->filmList.size() << endl;
            for (auto film : filmList)
            {
                sum += crypto::stringSum(film.name);
                stream << film.name << endl;
                sum += crypto::stringSum(film.data);
                stream << film.data << endl;
                sum += crypto::stringSum(film.time);
                stream << film.time << endl;
                sum += crypto::stringSum(reinterpret_cast<char*>(film.tickets));
                stream << film.tickets << endl;
                sum += film.FilmId;
                stream << film.FilmId << endl;
                sum += film.room;
                stream << film.room << endl;
                sum += film.cost;
                stream << film.cost << endl;
                sum += film.totalCost;
                stream << film.totalCost << endl;
            }
            stream << sum << endl;;
            stream.close();
            return true;
        }
        throw_exception("file open exception, terminating process", FilmManager::FilmManagerHandler, FILMDBWRITE_EX | error_type::critical_error, (void*)this);
        stream.close();
        return false;
    }

    bool ReadObject(ifstream& in)
    {
        uint64_t sum = 0;
        uint64_t fileSum = 0;
        if (in.is_open())
        {
            in >> this->currentFilmId;
            sum += this->currentFilmId;
            int size;
            in >> size;
            sum += size;
            for (int i = 0; i < size; ++i)
            {
                Film* film = new Film();
                in >> film->name;
                sum += crypto::stringSum(film->name);
                in >> film->data;
                sum += crypto::stringSum(film->data);
                in >> film->time;
                sum += crypto::stringSum(film->time);
                in >> film->tickets;
                sum += crypto::stringSum(reinterpret_cast<char*>(film->tickets));
                in >> film->FilmId;
                sum += film->FilmId;
                in >> film->room;
                sum += film->room;
                in >> film->cost;
                sum += film->cost;
                in >> film->totalCost;   
                sum += film->totalCost;
                this->filmList.push_back(*film);
                delete film;
            }
            in >> fileSum;
            if (sum == fileSum)
                return true;
            throw_exception("file open exception, try to resolve exception", FilmManager::FilmManagerHandler, FILMDBREAD_EX, (void*)this);
            return false;
        }
        throw_exception("file open exception, terminating process", FilmManager::FilmManagerHandler, FILMDBREAD_EX | error_type::critical_error, (void*)this);
        return false;
    }

    bool WriteObject(ofstream& out)
    {
        uint64_t sum = 0;
        if (out.is_open())
        {
            sum += currentFilmId;
            out << currentFilmId << endl;
            sum += this->filmList.size();
            out << this->filmList.size() << endl;
            for (auto film : filmList)
            {
                sum += crypto::stringSum(film.name);
                out << film.name << endl;
                sum += crypto::stringSum(film.data);
                out << film.data << endl;
                sum += crypto::stringSum(film.time);
                out << film.time << endl;
                sum += crypto::stringSum(reinterpret_cast<char*>(film.tickets));
                out << film.tickets << endl;
                sum += film.FilmId;
                out << film.FilmId << endl;
                sum += film.room;
                out << film.room << endl;
                sum += film.cost;
                out << film.cost << endl;
                sum += film.totalCost;
                out << film.totalCost << endl;
            }
            out << sum << endl;
            return true;
        }
        throw_exception("file open exception, terminating process", FilmManager::FilmManagerHandler, FILMDBWRITE_EX | error_type::critical_error, (void*)this);
        return false;
    }

    static void FilmManagerHandler(void* _this)
    {
        FilmManager* error_manager = (FilmManager*)_this;
        if (last_exception.ex_flags & error_type::critical_error)
            return;
        if (last_exception.ex_flags & FILMDBREAD_EX)
        {
            error_manager->currentFilmId = 0;
            error_manager->filmList.clear();
        }
        else if (last_exception.ex_flags & FILMMANAGMENT_EX) 
        {
            error_manager->WriteObject();
        }
    }

    inline int GetNewId()
    {
        return currentFilmId++;
    }
private:
    friend class UserManager;

    int currentFilmId = 0;

    list<Film> filmList;
};

#endif