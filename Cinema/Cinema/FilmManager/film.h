#ifndef FILM_H
#define FILM_H

#include <string>
#include <list>
#include "stdint.h"
#include "..\fileUtils\fileUtil.h"
#include "..\Crypto\cryptoUtil.h"

#define ROOM_CAPACITY           320
#define ROOM_ROWS               16
#define ROOM_PLACES_IN_A_ROW    20
#define EMPTY_PLACE             77
#define BOOKED_PLACE            88

class Film
{
public:
    Film()
    {
        this->tickets = new uint8_t[ROOM_CAPACITY + 1];
        memset(this->tickets, EMPTY_PLACE, ROOM_CAPACITY);
        (char)this->tickets[ROOM_CAPACITY] = '\0';
    }

    Film(const int& filmId, const string& name, const string& data, const string& time, uint8_t room)
    {
        this->FilmId = filmId;
        this->name = name;
        this->data = data;
        this->room = room;
        this->tickets = new uint8_t[ROOM_CAPACITY + 1];
        memset(this->tickets, EMPTY_PLACE, ROOM_CAPACITY);
        (char)this->tickets[ROOM_CAPACITY] = '\0';
    }

    Film(const Film& another)
    {
        this->cost = another.cost;
        this->data = another.data;
        this->time = another.time;
        this->name = another.name;
        this->totalCost = another.totalCost;
        this->FilmId = another.FilmId;
        this->room = another.room;
        this->tickets = new uint8_t[ROOM_CAPACITY + 1];
        memcpy(this->tickets, another.tickets, ROOM_CAPACITY + 1);
        (char)this->tickets[ROOM_CAPACITY] = '\0'; 
    }

    bool BuyTicket(int arrayOffset)
    {
        if (arrayOffset >= 0 && arrayOffset < ROOM_CAPACITY)
        {
            if (this->tickets[arrayOffset] == EMPTY_PLACE)
            {
                this->tickets[arrayOffset] = BOOKED_PLACE;
                this->totalCost += cost;
                return true;
            }
        }
        return false;
    }
    /*
        Row = [1..ROOM_ROWS]
        Place in a row = [1..ROOM_PLACES_IN_A_ROW]
    */
    bool BuyTicket(int row, int placeInARow)
    {
        if (row > 0 && row <= ROOM_ROWS
                && placeInARow >= 1 && placeInARow <= ROOM_PLACES_IN_A_ROW)
        {
            int arrayOffset = (row - 1) * ROOM_PLACES_IN_A_ROW + (placeInARow - 1);
            return this->BuyTicket(arrayOffset); 
        }
        return false;
    }

    bool BuyTicket()
    {
        char* _ptr = this->tickets;
        for (int i = 0; i < ROOM_CAPACITY; ++i)
        {
            if (*_ptr == EMPTY_PLACE)
            {
                *_ptr = BOOKED_PLACE;
                totalCost += cost;
                return true;
            }
        }
        return false;
    }

    bool operator==(const Film& lhs, const Film& rhs)
    {
        return lhs.FilmId == rhs.FilmId;
    }

    static inline int cmpByDate(const Film& lhs, const Film& rhs)
    {
        return strcmp(lhs.data.c_str(), rhs.data.c_str());
    }

    static inline int cmpByName(const Film& lhs, const Film& rhs)
    {
        return strcmp(lhs.name.c_str(), rhs.name.c_str());
    }

    static inline int cmpByTime(const Film& lhs, const Film& rhs)
    {
        return strcmp(lhs.time.c_str(), rhs.time.c_str());
    }

    static inline int cmpByTicketCost(const Film& lhs, const Film& rhs)
    {
        return lhs.cost - rhs.cost;
    }

    static inline int cmpByRoom(const Film& lhs, const Film& rhs)
    {
        return lhs.room - rhs.room;
    }

    static inline bool cmpByTotalCost(const Film& lhs, const Film& rhs)
    {
        return lhs.totalCost - rhs.totalCost;
    }

    ~Film()
    {
        delete[] tickets;
    }
public:
    int FilmId;

    string name;

    string data;

    string time;

    uint8_t room;

    int cost;
private:
    friend class FilmManager;

    int totalCost;

    uint8_t* tickets;
};

#endif